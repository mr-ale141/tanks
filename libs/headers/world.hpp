#pragma once
#include "bullet.hpp"
#include "tank.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <vector>

const sf::Clock clock;
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
enum typeTank
{
    userTank,
    enemyTank,
    enemyTankAI,
};

class World : public sf::Drawable
{
public:
    World(int width, int height);
    void updateWorld();

private:
    struct PRNG
    {
        std::mt19937 engine;
    };
    PRNG generator;
    float widthWorld;
    float heightWorld;
    Tank* user;
    Tank* enemis[MAX_ENEMIS];
    Tank* enemisAI[MAX_ENEMIS_AI];
    Bullet* bullets[MAX_BULLETS];

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void initGenerator(PRNG& generator);
    float getRandomFloat(float minValue, float maxValue);
    unsigned getRandomInt(unsigned minValue, unsigned maxValue);
};