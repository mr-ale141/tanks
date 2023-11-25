#pragma once
#include <SFML/Graphics.hpp>

const int BULLET_SPEED = 300;

class Bullet : public sf::Drawable
{
public:
    Bullet(
        sf::Vector2f startPosition,
        sf::Vector2f startDirection,
        bool isEnemyBullet,
        sf::Clock& clockRef
    );
    sf::Vector2f getPosition();
    sf::Vector2f getDirection();
    bool isEnemy;
    void update();

private:
    sf::Clock clock;
    sf::Texture bulletTexture;
    sf::Sprite bullet;
    sf::Vector2f position;
    sf::Vector2f direction;
    float preTime;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void updatePosition();
    void setBulletParameters(bool isEnemyBullet);
    void setOriginUpCenter();
    void setDirection(sf::Vector2f startDirection);
    void setPosition(sf::Vector2f newPosition);
};