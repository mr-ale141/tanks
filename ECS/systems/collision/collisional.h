#pragma ONCE
#include "../render/render.h"

void initCollisionalSystems(flecs::world& world)
{
    auto updateICantMove = world.system<sf::Sprite, Moving, Collisional>()
        .term<Bullet>().oper(flecs::Not)
        .each([&world](
                flecs::iter& it,
                size_t index,
                sf::Sprite& spriteSelf,
                Moving& moving,
                Collisional& collisional) {
            collisional.iCantMove = false;
            sf::Vector2f positionSelf = spriteSelf.getPosition();
            directionEnum directionSelf = moving.direction;

            world.filter_builder<sf::Sprite>()
                .term<Bullet>().oper(flecs::Not)
                .term<Fire>().oper(flecs::Not)
                .build()
                .each([&](flecs::entity eTarget, const sf::Sprite& spriteTarget) {
                    sf::Vector2f positionTarget = spriteTarget.getPosition();
                    sf::Vector2f directionForTarget = positionTarget - positionSelf;
                    float module = getModule(directionForTarget);
                    if (module != 0 && module <= SIZE_TANK)
                        collisional.iCantMove = iSee(
                                DIRECTIONS[directionSelf],
                                positionSelf,
                                positionTarget,
                                0.70);
                    if (module != 0 && (eTarget.has<User>() || eTarget.has<WallWood>() || eTarget.has<BaseUser>()) && iSee(
                            DIRECTIONS[directionSelf],
                            positionSelf,
                            positionTarget))
                    {
                        if (checkBarriers(
                                world.get<Render>(),
                                directionSelf,
                                moving.numPositionScreen,
                                getNumPosition(positionTarget),
                                eTarget.has<User>() || eTarget.has<BaseUser>()))
                        {
                            if (it.entity(index).has<Enemy>())
                                shootEnemy(it, index, spriteSelf, moving, *(it.entity(index).get_mut<Enemy>()));
                            else if (it.entity(index).has<EnemyAI>())
                                shootEnemyAI(it, index, spriteSelf, moving, *(it.entity(index).get_mut<EnemyAI>()));
                        }
                    }
                });

            if (!collisional.iCantMove)
                if (positionSelf.x < float(SIZE_TANK) / 2 && directionSelf == LEFT)
                    collisional.iCantMove = true;
                else if (positionSelf.x > WINDOW_WIDTH - float(SIZE_TANK) / 2 && directionSelf == RIGHT)
                    collisional.iCantMove = true;
                else if (positionSelf.y < float(SIZE_TANK) / 2 && directionSelf == UP)
                    collisional.iCantMove = true;
                else if (positionSelf.y > WINDOW_HEIGHT - float(SIZE_TANK) / 2 && directionSelf == DOWN)
                    collisional.iCantMove = true;

        });

    auto updateBullets = world.system<Collisional, Bullet, sf::Sprite, Moving>()
        .each([&](
                flecs::entity eBullet,
                Collisional& collisional,
                Bullet& bullet,
                sf::Sprite& spriteBullet,
                Moving& moving) {
            sf::Vector2f positionBullet = spriteBullet.getPosition();
            directionEnum directionBullet = moving.direction;
            int damageBullet = bullet.damage;
            bool iOverScreen = false;
            if (positionBullet.x < 0.f && directionBullet == LEFT)
                iOverScreen = true;
            else if (positionBullet.x > WINDOW_WIDTH && directionBullet == RIGHT)
                iOverScreen = true;
            else if (positionBullet.y < 0.f && directionBullet == UP)
                iOverScreen = true;
            else if (positionBullet.y > WINDOW_HEIGHT && directionBullet == DOWN)
                iOverScreen = true;

            if (iOverScreen)
                eBullet.destruct();
            else
            {
                world.filter_builder<sf::Sprite>()
                    .build()
                    .each([&](flecs::entity eTarget, const sf::Sprite& spriteTarget) {
                        sf::Vector2f positionTarget = spriteTarget.getPosition();
                        sf::Vector2f directionForTarget = positionTarget - positionBullet;
                        float module = getModule(directionForTarget);
                        if (module > float(SIZE_TANK) / 4 && module <= float(SIZE_TANK) / 2 && !eTarget.has<Fire>())
                            collisional.iCantMove = iSee(
                                    DIRECTIONS[directionBullet],
                                    positionBullet,
                                    positionTarget,
                                    0.50);
                        if (collisional.iCantMove)
                        {
                            auto liveTarget = eTarget.get_mut<Live>();
                            if (liveTarget)
                                liveTarget->hp -= damageBullet;
                            if (!bullet.isUser || !eTarget.has<Bullet>())
                            {
                                Fire fire = {};
                                fire.indexTexture = 0;
                                fire.preTimeUpdate = 0;
                                sf::Sprite sprite;
                                sprite.setPosition(positionTarget);
                                world.entity().set<Fire>(fire).set<sf::Sprite>(sprite);
                            }
                            eBullet.destruct();
                            collisional.iCantMove = false;
                        }
                    });
            }
        });

    auto destroyEntities = world.system<Live>()
        .term<WallMetal>().oper(flecs::Not)
        .each([&](flecs::entity e, Live& live) {
            if (live.hp <= 0)
            {
                auto render = world.get_mut<Render>();
                auto sprite = e.get<sf::Sprite>();
                auto numPosition = getNumPosition(sprite->getPosition());
                render->busyPositionScreen[numPosition] = false;
                if (e.has<User>())
                {
                    isWin = false;
                    world.get_mut<Render>()->window->close();
                }
                else
                    e.destruct();
            }
        });

    auto checkGameOver = world.system<Render>()
            .term_at(1).singleton()
            .each([&](Render& render) {
                auto userIsLive = world.count<User>();
                auto userBaseIsLive = world.count<BaseUser>();
                auto enemyBaseIsLive = world.count<BaseEnemy>();
                auto countEnemy = world.count<Enemy>();
                auto countEnemyAI = world.count<EnemyAI>();
                if ((countEnemy == 0 && countEnemyAI == 0) || enemyBaseIsLive == 0)
                {
//                    auto fire = world.count<Fire>();
//                    if (fire == 0)
//                    {
                        isWin = true;
                        render.window->close();
//                    }
                }
                else if (userIsLive == 0 || userBaseIsLive == 0)
                {
//                    auto fire = world.count<Fire>();
//                    if (fire == 0)
//                    {
                        isWin = false;
                        render.window->close();
//                    }
                }
            });

    updateICantMove.add(flecs::PostLoad);
    updateBullets.add(flecs::PostLoad);
    destroyEntities.add(flecs::PreUpdate);
    checkGameOver.add(flecs::OnStore);
}
