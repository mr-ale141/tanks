#include "headers/tank.hpp"
#include "headers/tankUser.hpp"
#include <iostream>

Tank::Tank(sf::Vector2f startPosition, sf::Vector2f startDirection, sf::Clock& clockRef)
{
    clock = clockRef;
    setPosition(startPosition);
    countHit = 0;
    timeLastShoot = 0.f;
    isDamaged = false;
    setDirection(startDirection);
    preTimeUpdatePosition = clock.getElapsedTime().asSeconds();
    preTimeUpdateDirection = clock.getElapsedTime().asSeconds();
}

void Tank::updatePosition()
{
    float currentTime = clock.getElapsedTime().asSeconds();
    float deltaTime = currentTime - preTimeUpdatePosition;
    float offset = speed * deltaTime;
    sf::Vector2f newPosition = position + direction * offset;
    setPosition(newPosition);
    preTimeUpdatePosition = currentTime;
}

void Tank::setPosition(sf::Vector2f newPosition)
{
    tank.setPosition(position = newPosition);
}

sf::Vector2f Tank::getDirection()
{
    return direction;
}

sf::Vector2f Tank::getPosition()
{
    return position;
}

void Tank::setDirection(sf::Vector2f newDirection)
{
    direction = newDirection;
    if (direction == DIRECTIONS[UP])
        tank.setRotation(0.f);
    else if (direction == DIRECTIONS[RIGHT])
        tank.setRotation(90.f);
    else if (direction == DIRECTIONS[DOWN])
        tank.setRotation(180.f);
    else if (direction == DIRECTIONS[LEFT])
        tank.setRotation(270.f);
    else
        std::cout << "Error: Unknow direction!!!\n";
}

void Tank::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(tank, states);
}

void Tank::setScaleTank()
{
    sf::Vector2f targetSize;
    sf::FloatRect size = tank.getGlobalBounds();
    float ratio = size.width / size.height;
    if (size.height > size.width)
        targetSize = { ratio * SIZE_TANK, SIZE_TANK };
    else
        targetSize = { SIZE_TANK, SIZE_TANK / ratio };
    tank.setScale(
        targetSize.x / size.width,
        targetSize.y / size.height);
}

void Tank::setOriginCenter()
{
    sf::FloatRect size = tank.getLocalBounds();
    tank.setOrigin({ size.width / 2, size.height / 2 });
}

float Tank::getSpeed()
{
    return speed;
}

void Tank::stop()
{
    speed = 0.f;
}

void Tank::drive()
{
    speed = speedMax;
}

void Tank::shoot(World& world)
{
    float currTime = clock.getElapsedTime().asSeconds();
    bool iSee = world.iSee(direction, position, world.user->getPosition());
    if (iSee && currTime - timeLastShoot > 1 / shootSpeed)
    {
        world.createBullet(position, direction, true);
        timeLastShoot = currTime;
    }
}

void Tank::destroy()
{
    stop();
    isDamaged = true;
}

void Tank::update(World& world)
{
    if (!isDamaged)
    {
        updatePosition();
        shoot(world);
    }
}
