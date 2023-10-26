#pragma once
#include <SFML/Graphics.hpp>

class Tank : public sf::Drawable
{
public:
    Tank(sf::Vector2f position, sf::Vector2f direction, sf::Vector2f speed);
    void setPosition(sf::Vector2f position);
    void setDirection(sf::Vector2f direction);
    void setSpeed(sf::Vector2f speed);
    sf::Vector2f getPosition();
    sf::Vector2f getDirection();
    sf::Vector2f getSpeed();

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::ConvexShape base;
    sf::CircleShape tower;
    sf::RectangleShape gun;
    sf::Vector2f position;
    sf::Vector2f speed;
    sf::Vector2f direction;
};
