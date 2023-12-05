#pragma ONCE
#include <SFML/Graphics.hpp>
#include <random>
#include "flecs.h"

constexpr unsigned WINDOW_WIDTH = 800;
constexpr unsigned WINDOW_HEIGHT = 600;
constexpr unsigned SIZE_TANK = 40;

constexpr unsigned ROW_COUNT = int(WINDOW_HEIGHT / SIZE_TANK);
constexpr unsigned COLUMN_COUNT = int(WINDOW_WIDTH / SIZE_TANK);
constexpr unsigned MAX_POSITION_IN_SCREEN = COLUMN_COUNT * ROW_COUNT;
constexpr unsigned NUM_POSITION_USER = (ROW_COUNT - 1) * COLUMN_COUNT + COLUMN_COUNT / 2 - 1;
const sf::Vector2f POSITION_USER = {
        float(SIZE_TANK) / 2 + NUM_POSITION_USER % COLUMN_COUNT * SIZE_TANK,
        float(SIZE_TANK) / 2 + NUM_POSITION_USER / COLUMN_COUNT * SIZE_TANK};

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

void initRender(flecs::world& world) {
    Render render;

    if (!render.userTexture.loadFromFile("../sprites/user.png"))
    {
        std::cout << "Error: I can't read texture \"../sprites/user.png\"!!!\n";
        exit(1);
    }

    if (!render.enemyTexture.loadFromFile("../sprites/enemy.png"))
    {
        std::cout << "Error: I can't read texture \"../sprites/enemy.png\"!!!\n";
        exit(1);
    }

    if (!render.enemyAiTexture.loadFromFile("../sprites/enemy_ai.png"))
    {
        std::cout << "Error: I can't read texture \"../sprites/enemy_ai.png\"!!!\n";
        exit(1);
    }

    if (!render.userBulletTexture.loadFromFile("../sprites/bullet_user.png"))
    {
        std::cout << "Error: I can't read texture \"../sprites/bullet_user.png\"!!!\n";
        exit(1);
    }

    if (!render.enemyBulletTexture.loadFromFile("../sprites/bullet_enemy.png"))
    {
        std::cout << "Error: I can't read texture \"../sprites/bullet_enemy.png\"!!!\n";
        exit(1);
    }

    if (!render.wallMetalTexture.loadFromFile("../sprites/wall_metal.png"))
    {
        std::cout << "Error: I can't read texture \"../sprites/wall_metal.png\"!!!\n";
        exit(1);
    }

    if (!render.wallWoodTexture_0.loadFromFile("../sprites/wall_wood_0.png"))
    {
        std::cout << "Error: I can't read texture \"../sprites/wall_wood_0.png\"!!!\n";
        exit(1);
    }

    if (!render.wallWoodTexture_1.loadFromFile("../sprites/wall_wood_1.png"))
    {
        std::cout << "Error: I can't read texture \"../sprites/wall_wood_1.png\"!!!\n";
        exit(1);
    }

    if (!render.wallWoodTexture_2.loadFromFile("../sprites/wall_wood_2.png"))
    {
        std::cout << "Error: I can't read texture \"../sprites/wall_wood_2.png\"!!!\n";
        exit(1);
    }

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    render.window = new sf::RenderWindow;
    render.window->create(sf::VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT }),
                          "Tanks",
                          sf::Style::Default,
                          settings);

    for (flecs::id &i : render.busyPositionScreen)
        i = flecs::id(0ULL);

    world.set<Render>(render);
}

void initRand(flecs::world& world)
{
    Rand rand;
    rand.initGenerator();
    world.set<Rand>(rand);
}

void initDrawSystem(flecs::world& world)
{
    initRender(world);
    initRand(world);

    world.system<Render>()
            .term_at(1).singleton()
            .each([](Render& render) {
                render.window->clear();
            }).add(flecs::PostUpdate);

    world.system<sf::Sprite, Render>()
            .term_at(2).singleton()
            .each([](sf::Sprite& sprite, Render& render) {
                render.window->draw(sprite);
            }).add(flecs::PreStore);

    world.system<Render>()
            .term_at(1).singleton()
            .each([](Render& render) {
                render.window->display();
            }).add(flecs::OnStore);
}