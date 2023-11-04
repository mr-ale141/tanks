#include "world.hpp"

World::World(int width, int height)
{
    widthWorld = float(width);
    heightWorld = float(height);
    initGenerator(generator);
    user = new Tank(userTank, { float(width / 2), float(height - SIZE_TANK / 2) }, DIRECTIONS[UP]);
    for (int i = 0; i < MAX_ENEMIS; i++)
    {
        sf::Vector2f position;
        position.x = getRandomFloat(SIZE_TANK / 2, widthWorld - SIZE_TANK / 2);
        position.y = getRandomFloat(SIZE_TANK / 2, heightWorld - SIZE_TANK / 2);
        int randomDirection = getRandomInt(0, 3);
        enemis[i] = new Tank(enemyTank, position, DIRECTIONS[randomDirection]);
    }
    for (int i = 0; i < MAX_ENEMIS_AI; i++)
    {
        sf::Vector2f position;
        position.x = getRandomFloat(SIZE_TANK / 2, widthWorld - SIZE_TANK / 2);
        position.y = getRandomFloat(SIZE_TANK / 2, heightWorld - SIZE_TANK / 2);
        int randomDirection = getRandomInt(0, 3);
        enemisAI[i] = new Tank(enemyTankAI, position, DIRECTIONS[randomDirection]);
    }
}

void World::updateWorld()
{
    user->updatePosition();
    for (int i = 0; i < MAX_ENEMIS; i++)
    {
        if (enemis[i])
            enemis[i]->updatePosition();
    }
    for (int i = 0; i < MAX_ENEMIS_AI; i++)
    {
        if (enemisAI[i])
            enemisAI[i]->updatePosition();
    }
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (bullets[i])
            bullets[i]->updatePosition();
    }
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
