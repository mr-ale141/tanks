#include "tank.hpp"

Tank::Tank(sf::Vector2f position, sf::Vector2f direction, sf::Vector2f speed)
{
    constexpr int radiusTower = 

    circle.setRadius(r);
    circle.setOrigin({ float(r), float(r) });
    circle.setPosition(pos);
    circle.setFillColor(c);
    circle.setOutlineColor(cL);
    circle.setOutlineThickness(5.f);

}

void Tank::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(base, states);
    target.draw(tower, states);
    target.draw(gun, states);
}