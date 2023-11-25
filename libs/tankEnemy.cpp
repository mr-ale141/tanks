#include "headers/tankEnemy.hpp"
#include <iostream>

TankEnemy::TankEnemy(sf::Vector2f startPosition, sf::Vector2f startDirection, sf::Clock& clockRef)
    : Tank(startPosition, startDirection, clockRef)
{
    if (!tankTexture.loadFromFile("./sprites/enemy.png"))
    {
        std::cout << "Error: I can't read texture \"./sprites/enemy.png\"!!!\n";
        exit(1);
    }
    speed = 0.f;
    speedMax = SPEED_ENEMY;
    shootSpeed = SHOOT_SPEED_ENEMY;
    tank.setTexture(tankTexture);
    setScaleTank();
    setOriginCenter();
}