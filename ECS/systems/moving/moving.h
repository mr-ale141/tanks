#pragma ONCE
#include "../collision/collisional.h"

void initMovingSystems(flecs::world& world)
{
    auto updateSpeeds = world.system<sf::Sprite, Moving, Collisional, Render>()
            .term<Bullet>().oper(flecs::Not)
            .term_at(4).singleton()
            .each([&](
                    flecs::entity e,
                    sf::Sprite& sprite,
                    Moving& moving,
                    Collisional& collisional,
                    Render& render) {

                if (collisional.iCantMove)
                {
                    moving.speed = 0.f;
                    float now = 0.f;
                    moving.nextTimeDirection = now;
                }
                else
                {
                    if (e.has<Enemy>())
                        moving.speed = SPEED_ENEMY;
                    else if (e.has<EnemyAI>())
                        moving.speed = SPEED_ENEMY_AI;
                }

            });

    auto updateDirections = world.system<sf::Sprite, Moving, Collisional, Render, Rand>()
            .term<User>().oper(flecs::Not)
            .term<Bullet>().oper(flecs::Not)
            .term_at(4).singleton()
            .term_at(5).singleton()
            .each([&](
                    flecs::entity e,
                    sf::Sprite& sprite,
                    Moving& moving,
                    Collisional& collisional,
                    Render& render,
                    Rand& rand) {

                float currentTime = render.clock.getElapsedTime().asSeconds();
                auto numPositionUser = world.lookup("User").get<Moving>()->numPositionScreen;
                if (e.has<Enemy>() && currentTime >= moving.nextTimeDirection)
                {
                    auto id = e.id();
                    moving.direction = getDirectionEnemy(rand, render, moving.numPositionScreen, numPositionUser, id);
                    moving.nextTimeDirection =
                            currentTime +
                            float(SIZE_TANK) *
                            1.f /
                            SPEED_ENEMY;
                    setDirection(sprite, moving.direction);
                    fixPositionInRange(sprite);
                }
                else if (e.has<EnemyAI>() && currentTime >= moving.nextTimeDirection)
                {
                    moving.direction = getDirectionEnemyAI(rand, render, moving.numPositionScreen, numPositionUser);
                    moving.nextTimeDirection =
                            currentTime +
                            float(SIZE_TANK) *
                            1.f /
                            SPEED_ENEMY_AI;
                    setDirection(sprite, moving.direction);
                    fixPositionInRange(sprite);
                }
            });

    auto updatePositions = world.system<sf::Sprite, Moving, Render>()
            .term_at(3).singleton()
            .each([](
                    sf::Sprite& sprite,
                    Moving& moving,
                    Render& render) {
                float currentTime = render.clock.getElapsedTime().asSeconds();
                float deltaTime = currentTime - moving.preTimeMoving;
                float offset = moving.speed * deltaTime;
                sf::Vector2f offsetPosition = DIRECTIONS[moving.direction] * offset;
                sf::Vector2f newPosition = sprite.getPosition() + offsetPosition;
                sprite.setPosition(newPosition);
                moving.preTimeMoving = currentTime;
            });

    auto updateFire = world.system<sf::Sprite, Fire, Render>()
            .term_at(3).singleton()
            .each([](flecs::entity e, sf::Sprite& sprite, Fire& fire, Render& render) {
                float currTime = render.clock.getElapsedTime().asSeconds();
                float dt = currTime - fire.preTimeUpdate;
                if (dt > STEP_UPDATE_FIRE)
                {
                    if (fire.indexTexture < COUNT_COLUMN_TEXTURE_FIRE)
                    {
                        sprite.setTexture(render.fire[fire.indexTexture]);
                        setOriginCenter(sprite);
                        fire.preTimeUpdate = currTime;
                        fire.indexTexture++;
                    }
                    else
                        e.destruct();
                }
            });

    auto updateWallWoodSprite = world.system<sf::Sprite, WallWood, Live, Render>()
            .term_at(4).singleton()
            .each([](flecs::entity e, sf::Sprite& sprite, WallWood, Live& live, Render& render) {
                if (live.hp == 2)
                    sprite.setTexture(render.wallWoodTexture_1);
                else if (live.hp == 1)
                    sprite.setTexture(render.wallWoodTexture_2);
            });

    auto updateBusyPosition = world.system<Moving, sf::Sprite, Render>()
            .term<Bullet>().oper(flecs::Not)
            .term_at(3).singleton()
            .each([](Moving& moving, sf::Sprite& sprite, Render& render) {
                auto currentPosition = sprite.getPosition();
                auto oldNum = moving.numPositionScreen;
                auto newNum = getNumPosition(currentPosition);
                if (oldNum != newNum)
                {
                    render.busyPositionScreen[oldNum] = false;
                    render.busyPositionScreen[newNum] = true;
                    moving.numPositionScreen = newNum;
                }
            });

    updateSpeeds.add(flecs::OnUpdate);
    updateDirections.add(flecs::OnUpdate);
    updatePositions.add(flecs::OnUpdate);
    updateFire.add(flecs::OnUpdate);
    updateWallWoodSprite.add(flecs::OnUpdate);
    updateBusyPosition.add(flecs::OnValidate);
}