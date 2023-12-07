#pragma ONCE
#include "config.h"

struct User {};

struct Enemy {};

struct EnemyAI {};

struct WallWood {};

struct WallMetal {};

struct Moving {
    directionEnum direction;
    float speed = 0.f;
    float nextTimeDirection = TIME_WAITING;
    float preTimeMoving;
    unsigned positionScreen;
};

struct Collision {
    bool iCantMove;
    bool isHitMe;
};

struct Render {
    sf::Texture userTexture;
    sf::Texture enemyTexture;
    sf::Texture enemyAiTexture;
    sf::Texture userBulletTexture;
    sf::Texture enemyBulletTexture;
    sf::Texture wallMetalTexture;
    sf::Texture wallWoodTexture_0;
    sf::Texture wallWoodTexture_1;
    sf::Texture wallWoodTexture_2;
    sf::RenderWindow* window;
    flecs::id busyPositionScreen[MAX_POSITION_IN_SCREEN];
    sf::Clock clock;
};

struct Rand {
    struct PRNG
    {
        std::mt19937 engine;
    };
    PRNG generator;
    void initGenerator()
    {
        std::random_device device;
        generator.engine.seed(device());
    }
    float getRandomFloat(float minValue, float maxValue)
    {
        if (minValue < maxValue)
        {
            std::uniform_real_distribution<float> distribution(minValue, maxValue);
            return distribution(generator.engine);
        }
        else
        {
            std::cout << "Error random FLOAT: minValue > maxValue !!!\n";
            exit(1);
        }
    }
    unsigned getRandomInt(unsigned minValue, unsigned maxValue)
    {
        if (minValue < maxValue)
        {
            std::uniform_int_distribution<unsigned> distribution(minValue, maxValue);
            return distribution(generator.engine);
        }
        else
        {
            std::cout << "Error random INT: minValue > maxValue !!!\n";
            exit(1);
        }
    }
};
