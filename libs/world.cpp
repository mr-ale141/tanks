#include "headers/world.hpp"
#include "headers/bullet.hpp"
#include "headers/tank.hpp"

World::World(int width, int height, sf::Clock& clockRef)
{
    clock = clockRef;
    widthWorld = float(width);
    heightWorld = float(height);
    initGenerator(generator);
    initBullets();
    createUser();
    createEnemis();
    createEnemisAI();
}

void World::initEnemis()
{
    for (int i = 0; i < MAX_ENEMIS; ++i)
        enemis[i] = NULL;
}

void World::initEnemisAI()
{
    for (int i = 0; i < MAX_ENEMIS_AI; ++i)
        enemisAI[i] = NULL;
}

void World::initBullets()
{
    for (int i = 0; i < MAX_BULLETS; i++)
        bullets[i] = NULL;
}

void World::createUser()
{
    user = new Tank(userTank, { float(widthWorld / 2), float(heightWorld - SIZE_TANK / 2) }, DIRECTIONS[UP], clock);
}

void World::createEnemis()
{
    initEnemis();
    for (int i = 0; i < MAX_ENEMIS; i++)
    {
        int randomDirection = getRandomInt(0, 3);
        sf::Vector2f position = getFreePosition();
        enemis[i] = new Tank(enemyTank, position, DIRECTIONS[randomDirection], clock);
    }
}

void World::createEnemisAI()
{
    initEnemisAI();
    for (int i = 0; i < MAX_ENEMIS_AI; i++)
    {
        int randomDirection = getRandomInt(0, 3);
        sf::Vector2f position = getFreePosition();
        enemisAI[i] = new Tank(enemyTankAI, position, DIRECTIONS[randomDirection], clock);
    }
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

sf::Vector2f World::getFreePosition()
{
    sf::Vector2f position = {widthWorld / 2, heightWorld / 2};
    bool isBusyPosition;
    const int MAX_TRYING = 500;
    int countTrying = 0;
    do
    {
        countTrying++;
        isBusyPosition = false;
        position.x = getRandomFloat(SIZE_TANK / 2, World::widthWorld - SIZE_TANK / 2);
        position.y = getRandomFloat(SIZE_TANK / 2, World::heightWorld / 2 - SIZE_TANK / 2);
        if (getModule((user->getPosition() - position)) < SIZE_TANK) isBusyPosition = true;
        for (int i = 0; i < MAX_ENEMIS_AI; ++i)
            if (enemisAI[i] != NULL && getModule((enemisAI[i]->getPosition() - position)) < SIZE_TANK)
                isBusyPosition = true;
        for (int i = 0; i < MAX_ENEMIS; ++i)
            if (enemis[i] != NULL && getModule((enemis[i]->getPosition() - position)) < SIZE_TANK)
                isBusyPosition = true;
    } while (isBusyPosition && countTrying < MAX_TRYING);
    return position;
}

void World::initGenerator(PRNG& generator)
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
    target.draw(*user, states);
    for (int i = 0; i < MAX_ENEMIS; i++)
    {
        if (enemis[i])
            target.draw(*enemis[i], states);
    }
    for (int i = 0; i < MAX_ENEMIS_AI; i++)
    {
        if (enemisAI[i])
            target.draw(*enemisAI[i], states);
    }
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (bullets[i])
            target.draw(*bullets[i], states);
    }
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

void World::movTankOutside(Tank* tank)
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
}

void World::update()
{
    user->update();
    movTankOutside(user);
    for (int i = 0; i < MAX_ENEMIS; i++)
    {
        if (enemis[i])
            enemis[i]->update();
        movTankOutside(enemis[i]);
    }
    for (int i = 0; i < MAX_ENEMIS_AI; i++)
    {
        if (enemisAI[i])
            enemisAI[i]->update();
        movTankOutside(enemisAI[i]);
    }
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (bullets[i])
        {
            bullets[i]->update();
            if (isOutside(bullets[i]))
            {
                free(bullets[i]);
                bullets[i] = NULL;
            }
            if (bullets[i] && !bullets[i]->isEnemy)
            {
                for (int j = 0; j < MAX_ENEMIS; j++)
                {
                    Tank* tankEnemy = enemis[j];
                    sf::Vector2f offset = tankEnemy->getPosition() - bullets[i]->getPosition();
                    if (getModule(offset) < SIZE_TANK / 2) 
                    {
                        tankEnemy->destroy();
                        free(bullets[i]);
                        bullets[i] = NULL;
                        break;
                    }
                }
            }
            if (bullets[i] && !bullets[i]->isEnemy)
            {
                for (int j = 0; j < MAX_ENEMIS_AI; j++)
                {
                    Tank* tankEnemy = enemisAI[j];
                    sf::Vector2f offset = tankEnemy->getPosition() - bullets[i]->getPosition();
                    if (getModule(offset) < SIZE_TANK / 2) 
                    {
                        tankEnemy->destroy();
                        free(bullets[i]);
                        bullets[i] = NULL;
                        break;
                    }
                }
            }
        }
    }
}