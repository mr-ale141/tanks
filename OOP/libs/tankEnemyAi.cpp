#include "headers/tankEnemyAi.hpp"
#include <iostream>

TankEnemyAi::TankEnemyAi(sf::Vector2f startPosition, sf::Vector2f startDirection, sf::Clock& clockRef)
    : Tank(startPosition, startDirection, clockRef)
{
    if (!tankTexture.loadFromFile("./sprites/enemy_ai.png"))
    {
        std::cout << "Error: I can't read texture \"./sprites/enemy_ai.png\"!!!\n";
        exit(1);
    }
    speed = 0.f;
    speedMax = SPEED_ENEMY_AI;
    shootSpeed = SHOOT_SPEED_ENEMY_AI;
    tank.setTexture(tankTexture);
    setScaleTank();
    setOriginCenter();
}