#pragma once
#include "world.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

static const int BULLET_SPEED = 100;

class Bullet : public sf::Drawable
{
public:
    Bullet(sf::Vector2f startPosition, sf::Vector2f startDirection, bool isEnemyBullet);
    sf::Vector2f getPosition();
    sf::Vector2f getDirection();
    void updatePosition();

private:
    sf::Texture bulletTexture;
    sf::Sprite bullet;
    sf::Vector2f position;
    sf::Vector2f direction;
    bool isEnemy;
    float preTime;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};