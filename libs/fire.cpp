#include "headers/fire.hpp"

Fire::Fire(sf::Clock& clockRef)
{
    clock = clockRef;
    initFireTexture();
    hiden = true;
    currFireIndex = 0;
    preTime = clock.getElapsedTime().asSeconds();
}

void Fire::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (!hiden)
        target.draw(fire[currFireIndex], states);
}

void Fire::setPosition(sf::Vector2f position)
{
    for (int i = 0; i < COUNT_SPRITE_FIRE; i++)
        fire[i].setPosition(position);
}

void Fire::show(sf::Vector2f position)
{
    setPosition(position);
    hiden = false;
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
        if (!fireTexture[indexTexture].loadFromFile("./sprites/fire.png", positionInTexture))
        {
            std::cout << "Error: I can't read texture \"./sprites/fire.png\"!!!\n";
            exit(1);
        }
        fire[indexTexture].setTexture(fireTexture[indexTexture]);
        fire[indexTexture].setOrigin({ SIZE_TEXTURE_FIRE / 2, SIZE_TEXTURE_FIRE / 2 });
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
    if (!hiden && dt > STEP_UPDATE_FIRE)
    {
        if (++currFireIndex >= COUNT_SPRITE_FIRE)
        {
            currFireIndex = 0;
        }
        preTime = clock.getElapsedTime().asSeconds();
    }
}