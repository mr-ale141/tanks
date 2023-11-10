#include "headers/fire.hpp"


Fire::Fire(sf::Clock& clockRef)
{
    clock = clockRef;
    indexFirePosition = 0;
    indexFireSprite = 0;
    initFireTexture();
    preTime = clock.getElapsedTime().asSeconds();
}

void Fire::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (int i = 0; i < indexFirePosition; i++)
    {
        fireSprites[indexFireSprite]->setPosition(positions[i]);
        target.draw(*fireSprites[indexFireSprite], states);
    }
}

void Fire::show(sf::Vector2f position)
{
    positions[indexFirePosition] = position;
    indexFirePosition++;
}

void Fire::initFireTexture()
{
    int currRow = 0;
    int currColumn = 0;
    for (int i = 0; i < COUNT_SPRITE_FIRE; i++)
    {
        sf::IntRect positionInTexture = {
            currColumn * SIZE_TEXTURE_FIRE,
            currRow * SIZE_TEXTURE_FIRE,
            SIZE_TEXTURE_FIRE,
            SIZE_TEXTURE_FIRE
        };
        int indexTexture = currRow * (MAX_COLUNN_TEXTURE_FIRE + 1) + currColumn;
        if (!fireTextures[indexTexture].loadFromFile("./sprites/fire.png", positionInTexture))
        {
            std::cout << "Error: I can't read texture \"./sprites/fire.png\"!!!\n";
            exit(1);
        }
        fireSprites[indexTexture] = new sf::Sprite;
        fireSprites[indexTexture]->setTexture(fireTextures[indexTexture]);
        fireSprites[indexTexture]->setOrigin({ SIZE_TEXTURE_FIRE / 2, SIZE_TEXTURE_FIRE / 2 });
        if (++currColumn > MAX_COLUNN_TEXTURE_FIRE)
        {
            currColumn = 0;
            ++currRow;
        }
    }
}

void Fire::update()
{
    float currTime = clock.getElapsedTime().asSeconds();
    float dt = currTime - preTime;
    if (dt > STEP_UPDATE_FIRE)
    {
        if (++indexFireSprite >= COUNT_SPRITE_FIRE)
        {
            indexFireSprite = 0;
        }
        preTime = clock.getElapsedTime().asSeconds();
    }
}