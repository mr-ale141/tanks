#pragma once
#include <SFML/Graphics.hpp>
#include <random>

const int MAX_ENEMIS = 10;
const int MAX_ENEMIS_AI = 5;
const int MAX_BULLETS = 20;
const sf::Vector2f TIME_RAND_DIRECTION = {2.f, 5.f};
const float TIME_WAITING = 3.f;

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

enum keyEnum
{
    spaceCode = 57,
    leftCode = 71,
    rightCode = 72,
    upCode = 73,
    downCode = 74,
};

class World : public sf::Drawable
{
public:
    class TankUser* user;

    World(int width, int height, sf::Clock& clockRef);
    void update();
    void updateEvent();
    void createBullet(sf::Vector2f position, sf::Vector2f direction, bool isEnemyBullet);
    float getRandomFloat(float minValue, float maxValue);
    unsigned getRandomInt(unsigned minValue, unsigned maxValue);
    float getModule(sf::Vector2f vector);

private:
    struct PRNG
    {
        std::mt19937 engine;
    };
    float widthWorld;
    float heightWorld;
    PRNG generator;
    sf::Clock clock;
    class TankEnemy* enemis[MAX_ENEMIS];
    class TankEnemyAi* enemisAI[MAX_ENEMIS_AI];
    class Bullet* bullets[MAX_BULLETS];
    class Fire* fire;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void initGenerator();
    sf::Vector2f getFreePosition();
    void initEnemis();
    void initEnemisAI();
    void initBullets();
    void createUser();
    void createEnemis();
    void createEnemisAI();
    void createFire();
    bool isOutside(Bullet* bullet);
    void movTankOutside(TankUser* tank);
    void rotateTankCollision(TankEnemy* tank);
    void rotateTankCollision(TankEnemyAi* tank);
    void updateUser();
    void updateEnemis();
    void updateEnemisAI();
    void updateBullets();
};