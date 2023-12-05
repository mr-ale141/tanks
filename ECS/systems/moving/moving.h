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
    float speed = 0.f;
    float nextTimeDirection = TIME_WAITING;
    float preTimeMoving;
    unsigned positionScreen;
    bool iCantMov = false;
};

float getModule(sf::Vector2f v)
{
    float module = std::sqrt(v.x * v.x + v.y * v.y);
    return module;
}

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

sf::Vector2f getPositionCenter(unsigned numPosition)
{
    sf::Vector2f position;
    unsigned rowNumber = numPosition / COLUMN_COUNT;
    unsigned columnNumber = numPosition % COLUMN_COUNT;
    position.x = float(SIZE_TANK) / 2 + float(columnNumber) * SIZE_TANK;
    position.y = float(SIZE_TANK) / 2 + float(rowNumber) * SIZE_TANK;
    return position;
}

unsigned getNextNumPosition(Moving& moving)
{
    unsigned oldNumPosition = moving.positionScreen;
    unsigned next = 0;
    directionEnum direction = moving.direction;
    switch (direction) {
        case UP:
            if (oldNumPosition < COLUMN_COUNT)
                next = MAX_POSITION_IN_SCREEN;
            else
                next = oldNumPosition - COLUMN_COUNT;
            break;
        case LEFT:
            if (!oldNumPosition % COLUMN_COUNT)
                next = MAX_POSITION_IN_SCREEN;
            else
                next = oldNumPosition - 1;
            break;
        case RIGHT:
            if (oldNumPosition % COLUMN_COUNT == COLUMN_COUNT - 1)
                next = MAX_POSITION_IN_SCREEN;
            else
                next = oldNumPosition + 1;
            break;
        case DOWN:
            if (oldNumPosition / COLUMN_COUNT == ROW_COUNT - 1)
                next = MAX_POSITION_IN_SCREEN;
            else
                next = oldNumPosition + COLUMN_COUNT;
            break;
    }
    return next;
}

void updateBusyPosition(sf::Vector2f newPosition, flecs::id id, Render& render, Moving& moving)
{
    sf::Vector2f positionCenter = getPositionCenter(moving.positionScreen);
    float offset = getModule(newPosition - positionCenter);
    if (offset >= SIZE_TANK / 2)
    {
        unsigned next = getNextNumPosition(moving);
        render.busyPositionScreen[moving.positionScreen] = flecs::id(0ULL);
        render.busyPositionScreen[next] = id;
        moving.positionScreen = next;
    }
}

void initMovingSystems(flecs::world& world)
{
    world.system<sf::Sprite, Moving, User, Render>()
            .term_at(4).singleton()
            .each([](
                    flecs::entity e,
                    sf::Sprite& sprite,
                    Moving& moving,
                    User,
                    Render& render) {

            }).add(flecs::OnUpdate);

    world.system<sf::Sprite, Moving, Enemy, Render, Rand>()
            .term_at(4).singleton()
            .term_at(5).singleton()
            .each([](
                    sf::Sprite& sprite,
                    Moving& moving,
                    Enemy,
                    Render& render,
                    Rand& rand) {
                float currentTime = render.clock.getElapsedTime().asSeconds();
                if (currentTime >= moving.nextTimeDirection) {
                    moving.speed = SPEED_ENEMY;
                    moving.direction = directionEnum(rand.getRandomInt(0, 3));
                    if (getNextNumPosition(moving) == MAX_POSITION_IN_SCREEN)
                        moving.speed = 0.f;
                    moving.nextTimeDirection =
                            currentTime +
                            SIZE_TANK *
                            rand.getRandomInt(
                                    RANGE_RAND_DIRECTION.x,
                                    RANGE_RAND_DIRECTION.y) /
                            SPEED_ENEMY;
                }
            }).add(flecs::OnUpdate);

    world.system<sf::Sprite, Moving, EnemyAI, Render, Rand>()
            .term_at(4).singleton()
            .term_at(5).singleton()
            .each([](
                    sf::Sprite& sprite,
                    Moving& moving,
                    EnemyAI,
                    Render& render,
                    Rand& rand) {
                float currentTime = render.clock.getElapsedTime().asSeconds();
                if (currentTime >= moving.nextTimeDirection) {
                    moving.speed = SPEED_ENEMY_AI;
                    moving.direction = directionEnum(rand.getRandomInt(0, 3));
                    if (getNextNumPosition(moving) == MAX_POSITION_IN_SCREEN)
                        moving.speed = 0.f;
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

                updateBusyPosition(newPosition, flecs::id(e.id()), render, moving);
                unsigned nextPosition = getNextNumPosition(moving);
                if (nextPosition == MAX_POSITION_IN_SCREEN && (
                        newPosition.x <= SIZE_TANK / 2 ||
                        newPosition.x >= WINDOW_WIDTH - SIZE_TANK / 2 ||
                        newPosition.y <= SIZE_TANK / 2 ||
                        newPosition.y >= WINDOW_HEIGHT - SIZE_TANK / 2
                        ))
                    moving.speed = 0.f;

            }).add(flecs::OnUpdate);
}