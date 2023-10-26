#include "ball.hpp"

Ball::Ball(
    int r,
    sf::Vector2f p,
    sf::Color c,
    sf::Color cL,
    const char* s,
    int chS,
    sf::Color chC)
{
    pos = p;
    v = { 0, 0 };
    circle.setRadius(r);
    circle.setOrigin({ float(r), float(r) });
    circle.setPosition(pos);
    circle.setFillColor(c);
    circle.setOutlineColor(cL);
    circle.setOutlineThickness(5.f);

    font.loadFromFile("arial.ttf");
    text.setFont(font);
    text.setString(s);
    text.setCharacterSize(chS);
    text.setFillColor(chC);
    sf::Vector2f sizeName = text.getLocalBounds().getSize();
    text.setOrigin({ sizeName.x / 2, (float)(sizeName.y / 1.2) });
    text.setPosition(pos);
}

void Ball::setPos(sf::Vector2f p)
{
    pos = p;
    circle.setPosition(p);
    text.setPosition(p);
}

sf::Vector2f Ball::getPos()
{
    return pos;
}

sf::Vector2f Ball::getV()
{
    return v;
}

void Ball::setV(sf::Vector2f speed)
{
    v = speed;
}

void Ball::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(circle, states);
    target.draw(text, states);
}