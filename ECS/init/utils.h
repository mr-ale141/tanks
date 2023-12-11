#pragma ONCE
#include "components.h"

void setScaleTank(sf::Sprite& sprite)
{
    sf::Vector2f targetSize;
    sf::FloatRect size = sprite.getGlobalBounds();
    float ratio = size.width / size.height;
    if (size.height > size.width)
        targetSize = { ratio * SIZE_TANK, SIZE_TANK };
    else
        targetSize = { SIZE_TANK, SIZE_TANK / ratio };
    sprite.setScale(
            targetSize.x / size.width,
            targetSize.y / size.height);
}

void setOriginCenter(sf::Sprite& sprite)
{
    sf::FloatRect size = sprite.getLocalBounds();
    sprite.setOrigin({ size.width / 2, size.height / 2 });
}

unsigned getFreePosition(Rand& rand, Render& render, int minNumPosition, int maxNumPosition)
{
    const int MAX_TRYING = 500;
    unsigned numPosition = 0;
    int countTrying = 0;
    do
    {
        countTrying++;
        numPosition = rand.getRandomInt(minNumPosition, maxNumPosition);
        if (numPosition == NUM_POSITION_USER)
            continue;
    } while (render.busyPositionScreen[numPosition] && countTrying < MAX_TRYING);
    if (countTrying == MAX_TRYING)
        std::cout << "All position is BUSY!!!\n";
    return numPosition;
}

float getModule(sf::Vector2f v)
{
    float module = std::sqrt(v.x * v.x + v.y * v.y);
    return module;
}

void setDirection(sf::Sprite& sprite, directionEnum direction)
{
    if (direction == UP)
        sprite.setRotation(0.f);
    else if (direction == RIGHT)
        sprite.setRotation(90.f);
    else if (direction == DOWN)
        sprite.setRotation(180.f);
    else if (direction == LEFT)
        sprite.setRotation(270.f);
    else
        std::cout << "Error: Unknown direction!!!\n";
}

sf::Vector2f getPositionCenter(unsigned numPosition)
{
    sf::Vector2f position;
    unsigned rowNumber = numPosition / COLUMN_COUNT;
    unsigned columnNumber = numPosition % COLUMN_COUNT;
    position.x = float(SIZE_TANK) / 2 + float(columnNumber) * SIZE_TANK;
    position.y = float(SIZE_TANK) / 2 + float(rowNumber) * SIZE_TANK;
    return position;
}

bool iSee(sf::Vector2f directionSelf, sf::Vector2f posSelf, sf::Vector2f posTarget, float sector = SECTOR_VIEW)
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

void fixPositionInRange(sf::Sprite& sprite)
{
    auto positionCurrent = sf::Vector2i(sprite.getPosition());
    int modX = (positionCurrent.x - int(SIZE_TANK / 2)) % int(SIZE_TANK);
    int modY = (positionCurrent.y - int(SIZE_TANK / 2)) % int(SIZE_TANK);

    if(modX)
    {
        if (modX > int(SIZE_TANK / 2))
        positionCurrent.x += int(SIZE_TANK) - modX;
        else
        positionCurrent.x -= modX;
        sprite.setPosition(sf::Vector2f(positionCurrent));
    }

    if(modY)
    {
        if (modY > int(SIZE_TANK / 2))
            positionCurrent.y += int(SIZE_TANK) - modY;
        else
            positionCurrent.y -= modY;
        sprite.setPosition(sf::Vector2f(positionCurrent));
    }
}
