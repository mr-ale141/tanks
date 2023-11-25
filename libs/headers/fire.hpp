#pragma once
#include <SFML/Graphics.hpp>

const int MAX_ROW_TEXTURE_FIRE = 7;
const int MAX_COLUNN_TEXTURE_FIRE = 15;
const int SIZE_TEXTURE_FIRE = 64;
const float STEP_UPDATE_FIRE = 0.05;
const int COUNT_SPRITE_FIRE = (MAX_ROW_TEXTURE_FIRE + 1) * (MAX_COLUNN_TEXTURE_FIRE + 1);
const int MAX_FIRE = 100;

class Fire : public sf::Drawable
{
public:
    Fire(sf::Clock& clockRef);
    void update();
    void show(sf::Vector2f position);

private:
    int maxIndexFirePosition;
    int indexFirePosition;
    int indexFireSprite;
    float preTime;
    sf::Clock clock;
    sf::Texture fireTextures[COUNT_SPRITE_FIRE];
    sf::Sprite* fireSprites[COUNT_SPRITE_FIRE];
    sf::Vector2f positions[MAX_FIRE];

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void initFireTexture();
};