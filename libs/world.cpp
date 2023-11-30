#include "headers/wallWood.hpp"
#include "headers/wallMetal.hpp"
#include "headers/wall.hpp"
#include "headers/fire.hpp"
#include "headers/bullet.hpp"
#include "headers/tank.hpp"
#include "headers/tankUser.hpp"
#include "headers/tankEnemy.hpp"
#include "headers/tankEnemyAi.hpp"
#include "headers/world.hpp"
#include <iostream>

World::World(int width, int height, sf::Clock& clockRef)
{
    clock = clockRef;
    widthWorld = float(width);
    heightWorld = float(height);
    rowCount = int(heightWorld / SIZE_TANK);
    columnCount = int(widthWorld / SIZE_TANK);
    maxPositionCount = rowCount * columnCount;
    initObjects();
    createObjects();
}

void World::initObjects()
{
    initGenerator();
    initBullets();
    initEnemies();
    initEnemiesAI();
    initWallMetal();
    initWallWood();
    initBullets();
}

void World::initWallMetal()
{
    for (int i = 0; i < MAX_WALL_METAL; ++i)
        wallsMetal[i] = NULL;
}

void World::initWallWood()
{
    for (int i = 0; i < MAX_WALL_WOOD; ++i)
        wallsWood[i] = NULL;
}

void World::initEnemies()
{
    for (int i = 0; i < MAX_Enemies; ++i)
        enemies[i] = NULL;
}

void World::initEnemiesAI()
{
    for (int i = 0; i < MAX_Enemies_AI; ++i)
        enemiesAI[i] = NULL;
}

void World::initBullets()
{
    for (int i = 0; i < MAX_BULLETS; i++)
        bullets[i] = NULL;
}

void World::createObjects()
{
    createUser();
    createEnemies();
    createEnemiesAI();
    createWallMetal();
    createWallWood();
    createFire();
}

void World::createWallMetal()
{
    for (int i = 0; i < MAX_WALL_METAL; i++)
    {
        sf::Vector2f position = getFreePosition(0, maxPositionCount - 1);
        wallsMetal[i] = new WallMetal(position);
    }
}

void World::createWallWood()
{
    for (int i = 0; i < MAX_WALL_WOOD; i++)
    {
        sf::Vector2f position = getFreePosition(0, maxPositionCount - 1);
        wallsWood[i] = new WallWood(position);
    }
}

void World::createUser()
{
    user = new TankUser({ float(widthWorld / 2  - SIZE_TANK / 2), float(heightWorld - SIZE_TANK / 2) }, DIRECTIONS[UP], clock);
}

void World::createEnemies()
{
    for (int i = 0; i < MAX_Enemies; i++)
    {
        int randomDirection = getRandomInt(0, 3);
        sf::Vector2f position = getFreePosition(0, int(rowCount / 2) * columnCount + columnCount - 1);
        enemies[i] = new TankEnemy(position, DIRECTIONS[randomDirection], clock);
        enemies[i]->stepRandomDirection = SIZE_TANK * getRandomInt(STEP_RAND_DIRECTION.x, STEP_RAND_DIRECTION.y) / SPEED_ENEMY;
    }
}

void World::createEnemiesAI()
{
    for (int i = 0; i < MAX_Enemies_AI; i++)
    {
        int randomDirection = getRandomInt(0, 3);
        sf::Vector2f position = getFreePosition(0, maxPositionCount / 2 - 1);
        enemiesAI[i] = new TankEnemyAi(position, DIRECTIONS[randomDirection], clock);
        enemiesAI[i]->stepRandomDirection = SIZE_TANK * getRandomInt(STEP_RAND_DIRECTION.x, STEP_RAND_DIRECTION.y) / SPEED_ENEMY_AI;
    }
}

void World::createFire()
{
    fire = new Fire(clock);
}

void World::createBullet(sf::Vector2f position, sf::Vector2f direction, bool isEnemyBullet)
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (bullets[i] == NULL)
        {
            bullets[i] = new Bullet(position, direction, isEnemyBullet, clock);
            break;
        }
    }
}

float World::getModule(sf::Vector2f v)
{
    float module = std::sqrt(v.x * v.x + v.y * v.y);
    return module;
}

sf::Vector2f World::getFreePosition(int minNumPosition, int maxNumPosition)
{
    sf::Vector2f position;
    bool isBusyPosition;
    const int MAX_TRYING = 500;
    const int numberPositionUser = 289;
    int numPosition;
    int countTrying = 0;
    do
    {
        countTrying++;
        isBusyPosition = false;
        numPosition = getRandomInt(minNumPosition, maxNumPosition);
        if (numPosition == numberPositionUser)
        {
            isBusyPosition = true;
            continue;
        }
        int rowNumber = numPosition / columnCount;
        int columnNumber = numPosition % columnCount;
        position.x = SIZE_TANK / 2 + columnNumber * SIZE_TANK;
        position.y = SIZE_TANK / 2 + rowNumber * SIZE_TANK;
        for (int i = 0; i < MAX_Enemies_AI; ++i)
        {
            if (enemiesAI[i] != NULL)
            {
                sf::Vector2f distance = enemiesAI[i]->getPosition() - position;
                float module = getModule(distance);
                if (module < SIZE_TANK)
                {
                    isBusyPosition = true;
                    break;
                }
            }
        }
        for (int i = 0; i < MAX_Enemies && !isBusyPosition; ++i)
            if (enemies[i] != NULL && getModule((enemies[i]->getPosition() - position)) < SIZE_TANK)
            {
                isBusyPosition = true;
                break;
            }
        for (int i = 0; i < MAX_WALL_METAL && !isBusyPosition; ++i)
            if (wallsMetal[i] != NULL && getModule((wallsMetal[i]->getPosition() - position)) < SIZE_TANK)
            {
                isBusyPosition = true;
                break;
            }
        for (int i = 0; i < MAX_WALL_WOOD && !isBusyPosition; ++i)
            if (wallsWood[i] != NULL && getModule((wallsWood[i]->getPosition() - position)) < SIZE_TANK)
            {
                isBusyPosition = true;
                break;
            }
    } while (isBusyPosition && countTrying < MAX_TRYING);
    return position;
}

void World::initGenerator()
{
    std::random_device device;
    generator.engine.seed(device());
}

float World::getRandomFloat(float minValue, float maxValue)
{
    if (minValue < maxValue)
    {
        std::uniform_real_distribution<float> distribution(minValue, maxValue);
        return distribution(generator.engine);
    }
    else
    {
        std::cout << "Error random FLOAT: minValue > maxValue !!!\n";
        exit(1);
    }
}

unsigned World::getRandomInt(unsigned minValue, unsigned maxValue)
{
    if (minValue < maxValue)
    {
        std::uniform_int_distribution<unsigned> distribution(minValue, maxValue);
        return distribution(generator.engine);
    }
    else
    {
        std::cout << "Error random INT: minValue > maxValue !!!\n";
        exit(1);
    }
}

void World::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (int i = 0; i < MAX_WALL_METAL; i++)
    {
        if (wallsMetal[i])
            target.draw(*wallsMetal[i], states);
    }
    for (int i = 0; i < MAX_WALL_WOOD; i++)
    {
        if (wallsWood[i])
            target.draw(*wallsWood[i], states);
    }
    target.draw(*user, states);
    for (int i = 0; i < MAX_Enemies; i++)
    {
        if (enemies[i])
            target.draw(*enemies[i], states);
    }
    for (int i = 0; i < MAX_Enemies_AI; i++)
    {
        if (enemiesAI[i])
            target.draw(*enemiesAI[i], states);
    }
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (bullets[i])
            target.draw(*bullets[i], states);
    }
    target.draw(*fire, states);
}

bool World::isOutside(Bullet* bullet)
{
    bool isOut = false;
    if (bullet->getPosition().x < 0 || bullet->getPosition().y < 0)
        isOut = true;
    if (bullet->getPosition().x > widthWorld || bullet->getPosition().y > heightWorld)
        isOut = true;
    return isOut;
}

void World::movTankOutside(TankUser* tank)
{
    sf::Vector2f position;
    if ((position = tank->getPosition()).x < 0)
        tank->setPosition({widthWorld, position.y});
    if ((position = tank->getPosition()).x > widthWorld)
        tank->setPosition({ 0, position.y });
    if ((position = tank->getPosition()).y < 0)
        tank->setPosition({ position.x, heightWorld });
    if ((position = tank->getPosition()).y > heightWorld)
        tank->setPosition({ position.x, 0 });
}

void World::rotateTankCollisionEdge(TankEnemy* tank)
{
    if (tank->getPosition().x < SIZE_TANK / 2 && tank->getDirection() == DIRECTIONS[LEFT])
        tank->setDirection(DIRECTIONS[getRandomInt(0, 3)]);
    if (tank->getPosition().x > widthWorld - SIZE_TANK / 2 && tank->getDirection() == DIRECTIONS[RIGHT])
        tank->setDirection(DIRECTIONS[getRandomInt(0, 3)]);
    if (tank->getPosition().y < SIZE_TANK / 2 && tank->getDirection() == DIRECTIONS[UP])
        tank->setDirection(DIRECTIONS[getRandomInt(0, 3)]);
    if (tank->getPosition().y > heightWorld - SIZE_TANK / 2 && tank->getDirection() == DIRECTIONS[DOWN])
        tank->setDirection(DIRECTIONS[getRandomInt(0, 3)]);
}

void World::rotateTankCollisionEdgeAI(TankEnemyAi* tank)
{
    if (tank->getPosition().x < SIZE_TANK / 2 && tank->getDirection() == DIRECTIONS[LEFT])
        tank->setDirection(DIRECTIONS[getRandomInt(0, 3)]);
    if (tank->getPosition().x > widthWorld - SIZE_TANK / 2 && tank->getDirection() == DIRECTIONS[RIGHT])
        tank->setDirection(DIRECTIONS[getRandomInt(0, 3)]);
    if (tank->getPosition().y < SIZE_TANK / 2 && tank->getDirection() == DIRECTIONS[UP])
        tank->setDirection(DIRECTIONS[getRandomInt(0, 3)]);
    if (tank->getPosition().y > heightWorld - SIZE_TANK / 2 && tank->getDirection() == DIRECTIONS[DOWN])
        tank->setDirection(DIRECTIONS[getRandomInt(0, 3)]);
}

void World::updateEvent()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        user->drive();
        user->setDirection(DIRECTIONS[UP]);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        user->drive();
        user->setDirection(DIRECTIONS[LEFT]);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        user->drive();
        user->setDirection(DIRECTIONS[RIGHT]);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        user->drive();
        user->setDirection(DIRECTIONS[DOWN]);
    }
    else
        user->stop();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        user->shoot(*this);
}

void World::updateUser()
{
    user->update(*this);
    movTankOutside(user);
}

void World::updateDirectionEnemy(int indexEnemy, bool isHard)
{
    if (enemies[indexEnemy])
    {
        float currTime = clock.getElapsedTime().asSeconds();
        float dt = currTime - enemies[indexEnemy]->preTimeUpdateDirection;
        if (dt >= enemies[indexEnemy]->stepRandomDirection || isHard)
        {
            int randomDirection = getRandomInt(0, 3);
            enemies[indexEnemy]->setDirection(DIRECTIONS[randomDirection]);
            enemies[indexEnemy]->stepRandomDirection =
                SIZE_TANK
                * getRandomInt(
                    STEP_RAND_DIRECTION.x,
                    STEP_RAND_DIRECTION.y)
                / SPEED_ENEMY;
            enemies[indexEnemy]->preTimeUpdateDirection = currTime;
        }
    }
}

void World::updateDirectionEnemyAI(int indexEnemyAI, bool isHard)
{
    if (enemiesAI[indexEnemyAI])
    {
        float currTime = clock.getElapsedTime().asSeconds();
        float dt = currTime - enemiesAI[indexEnemyAI]->preTimeUpdateDirection;
        if (dt >= enemiesAI[indexEnemyAI]->stepRandomDirection || isHard)
        {
            int randomDirection = getRandomInt(0, 3);
            enemiesAI[indexEnemyAI]->setDirection(DIRECTIONS[randomDirection]);
            enemiesAI[indexEnemyAI]->stepRandomDirection =
                SIZE_TANK
                * getRandomInt(
                    STEP_RAND_DIRECTION.x,
                    STEP_RAND_DIRECTION.y)
                / SPEED_ENEMY_AI;
            enemiesAI[indexEnemyAI]->preTimeUpdateDirection = currTime;
        }
    }
}

void World::updateEnemies()
{
    for (int indexEnemy = 0; indexEnemy < MAX_Enemies; indexEnemy++)
    {
        updateDirectionEnemy(indexEnemy);
        enemies[indexEnemy]->update(*this);
        rotateTankCollisionEdge(enemies[indexEnemy]);
        bool iCantMove = iBeforeWall(
            enemies[indexEnemy]->getPosition(),
            enemies[indexEnemy]->getDirection());
        if (iCantMove)
        {
            enemies[indexEnemy]->stop();
            updateDirectionEnemy(indexEnemy, true);
            iCantMove = iBeforeWall(
                enemies[indexEnemy]->getPosition(),
                enemies[indexEnemy]->getDirection());
        }
        if (!iCantMove && clock.getElapsedTime().asSeconds() > TIME_WAITING)
            enemies[indexEnemy]->drive();
    }
}

void World::updateEnemiesAI()
{
    for (int indexEnemyAI = 0; indexEnemyAI < MAX_Enemies_AI; indexEnemyAI++)
    {
        updateDirectionEnemyAI(indexEnemyAI);
        enemiesAI[indexEnemyAI]->update(*this);
        rotateTankCollisionEdgeAI(enemiesAI[indexEnemyAI]);
        bool iCantMov = iBeforeWall(
            enemiesAI[indexEnemyAI]->getPosition(),
            enemiesAI[indexEnemyAI]->getDirection());
        if (iCantMov)
        {
            enemiesAI[indexEnemyAI]->stop();
            updateDirectionEnemyAI(indexEnemyAI, true);
            iCantMov = iBeforeWall(
                enemiesAI[indexEnemyAI]->getPosition(),
                enemiesAI[indexEnemyAI]->getDirection());
        }
        if (!iCantMov && clock.getElapsedTime().asSeconds() > TIME_WAITING)
            enemiesAI[indexEnemyAI]->drive();
    }
}

void World::destroyEnemies(int indexBullet)
{
    for (int i = 0; i < MAX_Enemies; i++)
    {
        Tank* tankEnemy = enemies[i];
        sf::Vector2f position = tankEnemy->getPosition();
        sf::Vector2f offset = position - bullets[indexBullet]->getPosition();
        if (getModule(offset) < SIZE_TANK / 2)
        {
            tankEnemy->destroy();
            fire->show(position);
            delete bullets[indexBullet];
            bullets[indexBullet] = NULL;
            break;
        }
    }
}

void World::destroyEnemiesAI(int indexBulletAI)
{
    for (int i = 0; i < MAX_Enemies_AI; i++)
    {
        Tank* tankEnemy = enemiesAI[i];
        sf::Vector2f position = tankEnemy->getPosition();
        sf::Vector2f offset = position - bullets[indexBulletAI]->getPosition();
        if (getModule(offset) < SIZE_TANK / 2)
        {
            tankEnemy->destroy();
            fire->show(position);
            delete bullets[indexBulletAI];
            bullets[indexBulletAI] = NULL;
            break;
        }
    }
}

void World::updateBullets()
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (bullets[i])
        {
            bullets[i]->update();
            if (isOutside(bullets[i]))
            {
                delete bullets[i];
                bullets[i] = NULL;
            }
            if (bullets[i] && !bullets[i]->isEnemy)
                destroyEnemies(i);
            if (bullets[i] && !bullets[i]->isEnemy)
                destroyEnemiesAI(i);
        }
    }
}

void World::update()
{
    updateUser();
    fire->update();
    updateEnemies();
    updateEnemiesAI();
    updateBullets();
}

bool World::iSee(sf::Vector2f directionSelf, sf::Vector2f posSelf, sf::Vector2f posTarget, float sector)
{
    bool iSee = false;
    sf::Vector2f directionForTarget = posTarget - posSelf;
    float directionForTargetModule = getModule(directionForTarget);
    sf::Vector2f directionForUserNormal = {
        directionForTarget.x / directionForTargetModule,
        directionForTarget.y / directionForTargetModule
    };
    float scalar = directionForUserNormal.x * directionSelf.x + directionForUserNormal.y * directionSelf.y;
    if (scalar >= sector)
        iSee = true;
    return  iSee;
}

bool World::iBeforeWall(sf::Vector2f positionSelf, sf::Vector2f directionSelf)
{
    bool iBeforeWall = false;
    for (int indexWall = 0; indexWall < MAX_WALL_WOOD; indexWall++)
    {
        sf::Vector2f positionWall = wallsWood[indexWall]->getPosition();
        sf::Vector2f directionForTarget = positionWall - positionSelf;
        float module = getModule(directionForTarget);
        if (module <= SIZE_TANK)
        {
            bool isBeforeMe = iSee(directionSelf, positionSelf, positionWall);
            if (isBeforeMe)
            {
                iBeforeWall = true;
                break;
            }
        }
    }
    if (!iBeforeWall)
        for (int indexWallMetal = 0; indexWallMetal < MAX_WALL_METAL; indexWallMetal++)
        {
            sf::Vector2f positionWall = wallsMetal[indexWallMetal]->getPosition();
            sf::Vector2f directionForTarget = positionWall - positionSelf;
            float module = getModule(directionForTarget);
            if (module <= SIZE_TANK)
            {
                bool isBeforeMe = iSee(directionSelf, positionSelf, positionWall);
                if (isBeforeMe)
                {
                    iBeforeWall = true;
                    break;
                }
            }
        }
    return iBeforeWall;
}