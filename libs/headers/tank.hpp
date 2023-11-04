#pragma once
#include "world.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

const float SPEED_USER = 20.f;
const float SPEED_ENEMY_AI = 15.f;
const float SPEED_ENEMY = 10.f;
const float SHOOT_SPEED_USER = 2.f;
const float SHOOT_SPEED_ENEMY = 1 / 3;
const float SHOOT_SPEED_ENEMY_AI = 1.f;
const float SIZE_TANK = 40.f;

class Tank : public sf::Drawable
{
public:
    Tank(typeTank type, sf::Vector2f startPosition, sf::Vector2f startDirection);
    void updatePosition();
    void Tank::setDirection(sf::Vector2f newDirection);

private:
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
};
