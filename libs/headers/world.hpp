#pragma once
#include <SFML/Graphics.hpp>
#include <random>

const int MAX_Enemies = 10;
const int MAX_Enemies_AI = 5;
const int MAX_BULLETS = 20;
const int MAX_WALL_METAL = 30;
const int MAX_WALL_WOOD = 100;
const sf::Vector2i STEP_RAND_DIRECTION = {2, 10};
const float TIME_WAITING = 10.f;
const float SIZE_TANK = 40.f;
const float SECTOR_VIEW = 0.98;

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
    bool iSee(
        sf::Vector2f directionSelf,
        sf::Vector2f posSelf,
        sf::Vector2f posTarget,
        float sector = SECTOR_VIEW
    );

private:
    struct PRNG
    {
        std::mt19937 engine;
    };
    float widthWorld;
    float heightWorld;
    int maxPositionCount;
    int rowCount;
    int columnCount;
    PRNG generator;
    sf::Clock clock;
    class TankEnemy* enemies[MAX_Enemies];
    class TankEnemyAi* enemiesAI[MAX_Enemies_AI];
    class Bullet* bullets[MAX_BULLETS];
    class Fire* fire;
    class WallMetal* wallsMetal[MAX_WALL_METAL];
    class WallWood* wallsWood[MAX_WALL_WOOD];

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void initGenerator();
    sf::Vector2f getFreePosition(int minNumPosition, int maxNumPosition);
    void initObjects();
    void createObjects();
    void initEnemies();
    void initEnemiesAI();
    void initWallMetal();
    void initWallWood();
    void createWallMetal();
    void createWallWood();
    void initBullets();
    void createUser();
    void createEnemies();
    void createEnemiesAI();
    void createFire();
    bool isOutside(Bullet* bullet);
    void movTankOutside(TankUser* tank);
    void rotateTankCollisionEdge(TankEnemy* tank);
    void rotateTankCollisionEdgeAI(TankEnemyAi* tank);
    void updateUser();
    void updateDirectionEnemy(int indexEnemy, bool isHard = false);
    void updateDirectionEnemyAI(int indexEnemyAI, bool isHard = false);
    void updateEnemies();
    void updateEnemiesAI();
    void destroyEnemies(int indexBullet);
    void destroyEnemiesAI(int indexBulletAI);
    void updateBullets();
    bool iBeforeWall(sf::Vector2f positionSelf, sf::Vector2f directionSelf);
};