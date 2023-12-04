#pragma once
#include "world.hpp"
#include "tank.hpp"
#include <SFML/Graphics.hpp>

class TankUser : public Tank
{
public:
    TankUser(sf::Vector2f startPosition, sf::Vector2f startDirection, sf::Clock& clockRef);
    void shoot(World& world);
    void update(World& world);
};
