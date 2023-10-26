#pragma once
#include <SFML/Graphics.hpp>

class Ball : public sf::Drawable
{
public:
    Ball(int r, sf::Vector2f p, sf::Color c, sf::Color cL, const char* s, int chS, sf::Color chC);
    void setPos(sf::Vector2f v);
    sf::Vector2f getPos();
    sf::Vector2f getV();
    void setV(sf::Vector2f v);

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::CircleShape circle;
    sf::Text text;
    sf::Font font;
    sf::Vector2f pos;
    sf::Vector2f v;
};
