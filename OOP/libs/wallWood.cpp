#include "headers/wallWood.hpp"
#include <iostream>

WallWood::WallWood(sf::Vector2f createPosition)
    : Wall(createPosition)
{
    if (!wallTexture.loadFromFile("./sprites/wall_wood_0.png"))
    {
        std::cout << "Error: I can't read texture \"./sprites/wall_wood_0.png\"!!!\n";
        exit(1);
    }
    if (!wallTexture1.loadFromFile("./sprites/wall_wood_1.png"))
    {
        std::cout << "Error: I can't read texture \"./sprites/wall_wood_1.png\"!!!\n";
        exit(1);
    }
    if (!wallTexture2.loadFromFile("./sprites/wall_wood_2.png"))
    {
        std::cout << "Error: I can't read texture \"./sprites/wall_wood_2.png\"!!!\n";
        exit(1);
    }
    wall.setTexture(wallTexture);
    destroyed = true;
    hp = 3;
    setOriginCenter();
}

void WallWood::update()
{
    if (hp == 2)
        wall.setTexture(wallTexture1);
    if (hp == 1)
        wall.setTexture(wallTexture2);
}