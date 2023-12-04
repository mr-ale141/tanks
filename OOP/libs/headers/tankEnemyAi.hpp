#pragma once
#include "tank.hpp"
#include <SFML/Graphics.hpp>

class TankEnemyAi : public Tank
{
public:
    TankEnemyAi(sf::Vector2f startPosition, sf::Vector2f startDirection, sf::Clock& clockRef);
};