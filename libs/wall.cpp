#include "headers/wall.hpp"

Wall::Wall(sf::Vector2f createPosition)
{
    wall.setPosition(createPosition);
}

int Wall::destroy()
{
    if (destroyed)
    {
        --hp;
        update();
        return hp;
    }
    else
        return hp;
}

sf::Vector2f Wall::getPosition()
{
    return wall.getPosition();
}

void Wall::setOriginCenter()
{
    sf::FloatRect size = wall.getLocalBounds();
    wall.setOrigin({ size.width / 2, size.height / 2 });
}

void Wall::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(wall, states);
}

void Wall::update()
{

}