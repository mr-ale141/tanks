#include "headers/tankUser.hpp"
#include <iostream>

TankUser::TankUser(sf::Vector2f startPosition, sf::Vector2f startDirection, sf::Clock& clockRef)
    : Tank(startPosition, startDirection, clockRef)
{
    if (!tankTexture.loadFromFile("./sprites/user.png"))
    {
        std::cout << "Error: I can't read texture \"./sprites/user.png\"!!!\n";
        exit(1);
    }
    speed = 0.f;
    speedMax = SPEED_USER;
    shootSpeed = SHOOT_SPEED_USER;
    tank.setTexture(tankTexture);
    setScaleTank();
    setOriginCenter();
}

void TankUser::shoot(World& world)
{
    float currTime = clock.getElapsedTime().asSeconds();
    if (currTime - timeLastShoot > 1 / shootSpeed)
    {
        world.createBullet(position, direction, false);
        timeLastShoot = currTime;
    }
}

void TankUser::update(World& world)
{
    if (!isDamaged)
        updatePosition();
}