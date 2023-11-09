#pragma once
#include "world.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

// const float SPEED_USER = 20.f;
const float SPEED_USER = 80.f;
// const float SPEED_ENEMY_AI = 15.f;
const float SPEED_ENEMY_AI = 40.f;
// const float SPEED_ENEMY = 10.f;
const float SPEED_ENEMY = 60.f;
const float SHOOT_SPEED_USER = 2.f;
const float SHOOT_SPEED_ENEMY = 1 / 3;
const float SHOOT_SPEED_ENEMY_AI = 1.f;
const float SIZE_TANK = 40.f;

enum typeTank
{
    userTank,
    enemyTank,
    enemyTankAI,
};

class Tank : public sf::Drawable
{
public:
    Tank(typeTank type, sf::Vector2f startPosition, sf::Vector2f startDirection, sf::Clock& clockRef);
    sf::Vector2f getPosition();
    void setPosition(sf::Vector2f newPosition);
    sf::Vector2f getDirection();
    void setDirection(sf::Vector2f newDirection);
    float getSpeed();
    void shoot(World& world);
    void drive();
    void stop();
    void update();

private:
    typeTank type;
    sf::Clock clock;
    sf::Texture tankTexture;
    sf::Sprite tank;
    sf::Vector2f position;
    float speed;
    float shootSpeed;
    sf::Vector2f direction;
    int countHit;
    float preTime;
    float timeLastShoot;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void setTankParametrs(typeTank type);
    void setScaleTank();
    void setOriginCenter();
    void updatePosition();
};
