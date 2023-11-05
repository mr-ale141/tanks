#pragma once
#include "bullet.hpp"
#include "tank.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <vector>

const int MAX_ENEMIS = 5;
const int MAX_ENEMIS_AI = 1;
const int MAX_BULLETS = 20;
const sf::Vector2f DIRECTIONS[4] = {
    { 0.f, -1.f },
    { 1.f, 0.f },
    { 0.f, 1.f },
    { -1.f, 0.f }
};
enum directionEnum
{
    UP,
    RIGHT,
    DOWN,
    LEFT
};

class World : public sf::Drawable
{
public:

    World(int width, int height, sf::Clock& clockRef);
    void updateWorld();

private:
    struct PRNG
    {
        std::mt19937 engine;
    };
    PRNG generator;
    sf::Clock clock;
    float widthWorld;
    float heightWorld;
    class Tank* user;
    class Tank* enemis[MAX_ENEMIS];
    class Tank* enemisAI[MAX_ENEMIS_AI];
    class Bullet* bullets[MAX_BULLETS];

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void initGenerator(PRNG& generator);
    float getRandomFloat(float minValue, float maxValue);
    unsigned getRandomInt(unsigned minValue, unsigned maxValue);
};