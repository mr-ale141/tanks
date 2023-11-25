#include "headers/wallMetal.hpp"
#include <iostream>

WallMetal::WallMetal(sf::Vector2f createPosition)
    : Wall(createPosition)
{
    if (!wallTexture.loadFromFile("./sprites/wall_metal.png"))
    {
        std::cout << "Error: I can't read texture \"./sprites/wall_metal.png\"!!!\n";
        exit(1);
    }
    wall.setTexture(wallTexture);
    destroyed = false;
    int notDestroyed = -1;
    hp = notDestroyed;
    setOriginCenter();
}
