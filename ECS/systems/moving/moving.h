#pragma ONCE
#include <string>
#include <iostream>
#include "../render/render.h"

const float SPEED_USER = 80.f;
const float SPEED_ENEMY_AI = 20.f;
const float SPEED_ENEMY = 40.f;
const float TIME_WAITING = 5.f;
const sf::Vector2i RANGE_RAND_DIRECTION = {1, 4};
const sf::Vector2f DIRECTIONS[4] = {
        { 0.f, -1.f },
        { 1.f, 0.f },
        { 0.f, 1.f },
        { -1.f, 0.f }
};

enum directionEnum
{
    UP,
    RIGHT,
    DOWN,
    LEFT
};

struct Moving {
    directionEnum direction;
    float speed;
    float nextTimeDirection;
    float preTimeMoving;
    unsigned positionScreen;
};

void setDirection(sf::Sprite& sprite, directionEnum direction)
{
    if (direction == UP)
        sprite.setRotation(0.f);
    else if (direction == RIGHT)
        sprite.setRotation(90.f);
    else if (direction == DOWN)
        sprite.setRotation(180.f);
    else if (direction == LEFT)
        sprite.setRotation(270.f);
    else
        std::cout << "Error: Unknown direction!!!\n";
}

void initMovingSystem(flecs::world& world)
{
    world.system<sf::Sprite, Moving, Render, Rand>()
            .term_at(3).singleton()
            .term_at(4).singleton()
            .each([](
                    flecs::entity e,
                    sf::Sprite& sprite,
                    Moving& moving,
                    Render& render,
                    Rand& rand) {
                std::string typesStr = std::string (e.type().str());
                float currentTime = render.clock.getElapsedTime().asSeconds();
                float deltaTime = currentTime - moving.preTimeMoving;
                float offset = moving.speed * deltaTime;
                sf::Vector2f offsetPosition = DIRECTIONS[moving.direction] * offset;
                sf::Vector2f newPosition = sprite.getPosition() + offsetPosition;
                sprite.setPosition(newPosition);

                moving.preTimeMoving = currentTime;
                if (typesStr.find("User") != std::string::npos)
                {
                    ;
                }
                else if (typesStr.find("EnemyAI") != std::string::npos)
                {
                    if (moving.speed == 0.f && currentTime > TIME_WAITING)
                    {
                        moving.speed = SPEED_ENEMY_AI;
                        moving.nextTimeDirection =
                                currentTime +
                                SIZE_TANK *
                                rand.getRandomInt(
                                        RANGE_RAND_DIRECTION.x,
                                        RANGE_RAND_DIRECTION.y) /
                                SPEED_ENEMY_AI;
                    } else if (moving.speed != 0.f && currentTime >= moving.nextTimeDirection) {
                        moving.direction = directionEnum(rand.getRandomInt(0, 3));
                        setDirection(sprite, moving.direction);
                        moving.nextTimeDirection =
                                currentTime +
                                SIZE_TANK *
                                rand.getRandomInt(
                                        RANGE_RAND_DIRECTION.x,
                                        RANGE_RAND_DIRECTION.y) /
                                SPEED_ENEMY_AI;
                    }
                }
                else if (typesStr.find("Enemy") != std::string::npos)
                {
                    if (moving.speed == 0.f && currentTime > TIME_WAITING)
                    {
                        moving.speed = SPEED_ENEMY;
                        moving.nextTimeDirection =
                                currentTime +
                                SIZE_TANK *
                                rand.getRandomInt(
                                        RANGE_RAND_DIRECTION.x,
                                        RANGE_RAND_DIRECTION.y) /
                                SPEED_ENEMY_AI;
                    } else if (moving.speed != 0.f && currentTime >= moving.nextTimeDirection) {
                        moving.direction = directionEnum(rand.getRandomInt(0, 3));
                        setDirection(sprite, moving.direction);
                        moving.nextTimeDirection =
                                currentTime +
                                SIZE_TANK *
                                rand.getRandomInt(
                                        RANGE_RAND_DIRECTION.x,
                                        RANGE_RAND_DIRECTION.y) /
                                SPEED_ENEMY_AI;
                    }
                }
            }).add(flecs::OnUpdate);
}