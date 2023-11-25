#pragma once
#include "wall.hpp"
#include <SFML/Graphics.hpp>

class WallWood : public Wall
{
public:
    WallWood(sf::Vector2f createPosition);
    void update();

private:
    sf::Texture wallTexture1;
    sf::Texture wallTexture2;
};
