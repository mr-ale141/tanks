#pragma once
#include "tank.hpp"
#include <SFML/Graphics.hpp>

class TankEnemy : public Tank
{
public:
    TankEnemy(sf::Vector2f startPosition, sf::Vector2f startDirection, sf::Clock& clockRef);
};