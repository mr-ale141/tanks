#pragma once
#include "world.hpp"
#include <SFML/Graphics.hpp>

const float SPEED_USER = 80.f;
const float SPEED_ENEMY_AI = 20.f;
const float SPEED_ENEMY = 40.f;
const float SHOOT_SPEED_USER = 2.f;
const float SHOOT_SPEED_ENEMY = 1.f / 3.f;
const float SHOOT_SPEED_ENEMY_AI = 1.f / 4.f;
const float SIZE_TANK = 40.f;
const int HP_USER = 3;
const int HP_ENEMY = 1;
const int HP_ENEMY_AI = 2;
const float SECTOR_VIEW = 0.98;

class Tank : public sf::Drawable
{
public:
    Tank(sf::Vector2f startPosition, sf::Vector2f startDirection, sf::Clock& clockRef);

    float stepRandomDirection;
    float preTimeUpdateDirection;

    sf::Vector2f getPosition();
    void setPosition(sf::Vector2f newPosition);
    sf::Vector2f getDirection();
    void setDirection(sf::Vector2f newDirection);
    float getSpeed();
    void stop();
    void destroy();
    void drive();
    virtual void shoot(World& world);
    virtual void update(World& world);

private:


protected:
    float speed;
    float speedMax;
    float shootSpeed;
    int countHit;
    float preTimeUpdatePosition;
    float timeLastShoot;
    bool isDamaged;

    sf::Texture tankTexture;
    sf::Sprite tank;
    sf::Clock clock;
    sf::Vector2f position;
    sf::Vector2f direction;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void setScaleTank();
    void setOriginCenter();
    void updatePosition();
};
