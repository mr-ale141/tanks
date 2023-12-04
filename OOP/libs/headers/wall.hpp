#pragma once
#include <SFML/Graphics.hpp>

const float WALL_SIZE = 40;

class Wall : public sf::Drawable
{
public:
    Wall(sf::Vector2f createPosition);
    sf::Vector2f getPosition();
    int destroy();
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    virtual void update();

protected:
    bool destroyed;
    int hp;
    sf::Texture wallTexture;
    sf::Sprite wall;
    void setOriginCenter();
};