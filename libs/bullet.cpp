#include "headers/bullet.hpp"

Bullet::Bullet(sf::Vector2f startPosition, sf::Vector2f startDirection, bool isEnemyBullet)
{
    isEnemy = isEnemyBullet;
    if (isEnemy)
    {
        if (!bulletTexture.loadFromFile("./sprites/bullet_enemy.png"))
        {
            std::cout << "Error: I can't read texture \"./sprites/bullet_enemy.png\"!!!\n";
            exit(1);
        }
    }
    else 
    {
        if (!bulletTexture.loadFromFile("./sprites/bullet_user.png"))
        {
            std::cout << "Error: I can't read texture \"./sprites/bullet_user.png\"!!!\n";
            exit(1);
        }
    }
    bullet.setTexture(bulletTexture);

    sf::FloatRect size = bullet.getGlobalBounds();
    bullet.setOrigin({ size.width / 2, 0 });

    direction = startDirection;
    if (direction == DIRECTIONS[UP])
        bullet.setRotation(0.f);
    else if (direction == DIRECTIONS[RIGHT])
        bullet.setRotation(90.f);
    else if (direction == DIRECTIONS[DOWN])
        bullet.setRotation(180.f);
    else if (direction == DIRECTIONS[LEFT])
        bullet.setRotation(270.f);
    else
        std::cout << "Error: Unknow direction!!!\n";
    position = startPosition;
    bullet.setPosition(startPosition);
}

sf::Vector2f Bullet::getPosition()
{
    return position;
}

sf::Vector2f Bullet::getDirection()
{
    return direction;
}

void Bullet::updatePosition()
{
    float currentTime = clock.getElapsedTime().asSeconds();
    float deltaTime = currentTime - preTime;
    float offset = BULLET_SPEED * deltaTime;
    position += direction * offset;
    bullet.setPosition(position);
}

void Bullet::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(bullet, states);
}