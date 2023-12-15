#pragma ONCE
#include "../collision/collisional.h"

void initMovingSystems(flecs::world& world)
{
    world.system<sf::Sprite, Moving, Collisional, Render>()
            .term<Bullet>().oper(flecs::Not)
            .term_at(4).singleton()
            .each([](
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

            }).add(flecs::OnUpdate);

    world.system<sf::Sprite, Moving, Collisional, Render, Rand>()
            .term_at(4).singleton()
            .term_at(5).singleton()
            .each([](
                    flecs::entity e,
                    sf::Sprite& sprite,
                    Moving& moving,
                    Collisional& collisional,
                    Render& render,
                    Rand& rand) {

                float currentTime = render.clock.getElapsedTime().asSeconds();

                float speed;
                if (e.has<Enemy>())
                    speed = SPEED_ENEMY;
                else if (e.has<EnemyAI>())
                    speed = SPEED_ENEMY_AI;

                if (!e.has<User>() && currentTime >= moving.nextTimeDirection) {
                    moving.direction = directionEnum(rand.getRandomInt(0, 3));
                    moving.nextTimeDirection =
                            currentTime +
                            float(SIZE_TANK) *
                            float(rand.getRandomInt(
                                    RANGE_RAND_DIRECTION.x,
                                    RANGE_RAND_DIRECTION.y)) /
                            speed;
                    setDirection(sprite, moving.direction);
                    fixPositionInRange(sprite);
                }
            }).add(flecs::OnUpdate);

    world.system<sf::Sprite, Moving, Render>()
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
            }).add(flecs::OnUpdate);
}