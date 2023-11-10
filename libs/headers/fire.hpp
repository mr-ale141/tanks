#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

const int MAX_ROW_TEXTURE_FIRE = 7;
const int MAX_COLUNN_TEXTURE_FIRE = 15;
const int SIZE_TEXTURE_FIRE = 64;
const float STEP_UPDATE_FIRE = 0.05;
const int COUNT_SPRITE_FIRE = (MAX_ROW_TEXTURE_FIRE + 1) * (MAX_COLUNN_TEXTURE_FIRE + 1);

class Fire : public sf::Drawable
{
public:
    Fire(sf::Clock& clockRef);
    void update();
    void show(sf::Vector2f position);

private:
    sf::Clock clock;
    sf::Texture fireTexture[COUNT_SPRITE_FIRE];
    sf::Sprite fire[COUNT_SPRITE_FIRE];
    sf::Vector2f position;
    bool hiden;
    float preTime;
    int currFireIndex;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void initFireTexture();
    void setPosition(sf::Vector2f position);
};