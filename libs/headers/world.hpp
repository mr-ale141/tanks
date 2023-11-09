#pragma once
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
    class Tank* user;

    World(int width, int height, sf::Clock& clockRef);
    void update();
    void updateEvent();
    void createBullet(sf::Vector2f position, sf::Vector2f direction, bool isEnemyBullet);

private:
    struct PRNG
    {
        std::mt19937 engine;
    };
    float widthWorld;
    float heightWorld;
    PRNG generator;
    sf::Clock clock;
    class Tank* enemis[MAX_ENEMIS];
    class Tank* enemisAI[MAX_ENEMIS_AI];
    class Bullet* bullets[MAX_BULLETS];

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void initGenerator(PRNG& generator);
    float getRandomFloat(float minValue, float maxValue);
    unsigned getRandomInt(unsigned minValue, unsigned maxValue);
    sf::Vector2f getFreePosition();
    float getModule(sf::Vector2f vector);
    void initEnemis();
    void initEnemisAI();
    void initBullets();
    void createUser();
    void createEnemis();
    void createEnemisAI();
    bool isOutside(Bullet* bullet);
    void movTankOutside(Tank* tank);
};