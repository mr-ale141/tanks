#pragma ONCE
#include "../collision/collision.h"

void initMovingSystems(flecs::world& world)
{
    world.system<sf::Sprite, Moving, Collision, User, Render>()
            .term_at(5).singleton()
            .each([](
                    flecs::entity e,
                    sf::Sprite& sprite,
                    Moving& moving,
                    Collision& collision,
                    User,
                    Render& render) {

                if (collision.iCantMove)
                {
                    moving.speed = 0.f;
                    collision.iCantMove = false;
                }

            }).add(flecs::OnUpdate);

    world.system<sf::Sprite, Moving, Collision, Enemy, Render, Rand>()
            .term_at(5).singleton()
            .term_at(6).singleton()
            .each([](
                    sf::Sprite& sprite,
                    Moving& moving,
                    Collision& collision,
                    Enemy,
                    Render& render,
                    Rand& rand) {

                float currentTime = render.clock.getElapsedTime().asSeconds();

                if (collision.iCantMove)
                {
                    moving.speed = 0.f;
                    collision.iCantMove = false;
                    moving.nextTimeDirection = currentTime;
                }

                if (currentTime >= moving.nextTimeDirection) {
                    moving.speed = SPEED_ENEMY;
                    moving.direction = directionEnum(rand.getRandomInt(0, 3));
                    moving.nextTimeDirection =
                            currentTime +
                            SIZE_TANK *
                            rand.getRandomInt(
                                    RANGE_RAND_DIRECTION.x,
                                    RANGE_RAND_DIRECTION.y) /
                            SPEED_ENEMY;
                }
            }).add(flecs::OnUpdate);

    world.system<sf::Sprite, Moving, Collision, EnemyAI, Render, Rand>()
            .term_at(5).singleton()
            .term_at(6).singleton()
            .each([](
                    sf::Sprite& sprite,
                    Moving& moving,
                    Collision& collision,
                    EnemyAI,
                    Render& render,
                    Rand& rand) {

                float currentTime = render.clock.getElapsedTime().asSeconds();

                if (collision.iCantMove)
                {
                    moving.speed = 0.f;
                    collision.iCantMove = false;
                    moving.nextTimeDirection = currentTime;
                }

                if (currentTime >= moving.nextTimeDirection) {
                    moving.speed = SPEED_ENEMY_AI;
                    moving.direction = directionEnum(rand.getRandomInt(0, 3));
                    moving.nextTimeDirection =
                            currentTime +
                            SIZE_TANK *
                            rand.getRandomInt(
                                    RANGE_RAND_DIRECTION.x,
                                    RANGE_RAND_DIRECTION.y) /
                            SPEED_ENEMY_AI;
                }
            }).add(flecs::OnUpdate);

    world.system<sf::Sprite, Moving, Render>()
            .term_at(3).singleton()
            .each([](
                    flecs::entity e,
                    sf::Sprite& sprite,
                    Moving& moving,
                    Render& render) {
                setDirection(sprite, moving.direction);
                float currentTime = render.clock.getElapsedTime().asSeconds();
                float deltaTime = currentTime - moving.preTimeMoving;
                float offset = moving.speed * deltaTime;
                sf::Vector2f offsetPosition = DIRECTIONS[moving.direction] * offset;
                sf::Vector2f newPosition = sprite.getPosition() + offsetPosition;
                sprite.setPosition(newPosition);
                moving.preTimeMoving = currentTime;
            }).add(flecs::OnUpdate);
}