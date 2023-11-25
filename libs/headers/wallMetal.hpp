#pragma once
#include "wall.hpp"
#include <SFML/Graphics.hpp>

class WallMetal : public Wall
{
public:
    WallMetal(sf::Vector2f createPosition);
};