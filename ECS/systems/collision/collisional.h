#pragma ONCE
#include "../render/render.h"

void initCollisional(flecs::world& world)
{
    world.system<sf::Sprite, Moving, Collisional>()
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
                    if (module != 0 && eTarget.has<User>() && iSee(
                            DIRECTIONS[directionSelf],
                            positionSelf,
                            positionTarget,
                            0.99))
                        if (it.entity(index).has<Enemy>())
                            shootEnemy(it, index, spriteSelf, moving, *(it.entity(index).get_mut<Enemy>()));
                        else if (it.entity(index).has<EnemyAI>())
                            shootEnemyAI(it, index, spriteSelf, moving, *(it.entity(index).get_mut<EnemyAI>()));
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

        }).add(flecs::PreUpdate);

    world.system<Collisional, Bullet, sf::Sprite, Moving>()
        .each([&](
                flecs::entity eBullet,
                Collisional& collisional,
                Bullet& bullet,
                sf::Sprite& spriteBullet,
                Moving& moving) {
            sf::Vector2f positionBullet = spriteBullet.getPosition();
            directionEnum directionSelf = moving.direction;
            bool iOverScreen = false;
            if (positionBullet.x < 0.f && directionSelf == LEFT)
                iOverScreen = true;
            else if (positionBullet.x > WINDOW_WIDTH && directionSelf == RIGHT)
                iOverScreen = true;
            else if (positionBullet.y < 0.f && directionSelf == UP)
                iOverScreen = true;
            else if (positionBullet.y > WINDOW_HEIGHT && directionSelf == DOWN)
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
                        if (module > 0 && module <= SIZE_TANK / 2)
                            collisional.iCantMove = iSee(
                                    DIRECTIONS[directionSelf],
                                    positionBullet,
                                    positionTarget,
                                    0.50);
                        if (collisional.iCantMove)
                        {
                            if (!eTarget.has<WallMetal>())
                                eTarget.destruct();
                            eBullet.destruct();
                            collisional.iCantMove = false;
                        }
                    });
            }

        }).add(flecs::PreUpdate);
}