#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <random>
#include "flecs.h"

constexpr unsigned WINDOW_WIDTH = 800;
constexpr unsigned WINDOW_HEIGHT = 600;
constexpr unsigned MAX_ENEMY = 10;
constexpr unsigned MAX_ENEMY_AI = 5;
constexpr unsigned MAX_WALL_WOOD = 50;
constexpr unsigned MAX_WALL_METAL = 10;
constexpr unsigned SIZE_TANK = 40;

//void init()
//{
//}

//void pollEvents(sf::RenderWindow& window)
//{
//    sf::Event event;
//    while (window.pollEvent(event))
//    {
//        switch (event.type)
//        {
//            case sf::Event::Closed:
//                window.close();
//                break;
//            default:
//                break;
//        }
//    }
//}

//void update()
//{
//}

//void redrawFrame(sf::RenderWindow& window)
//{
//    window.clear();
//    window.draw();
//    window.display();
//}

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
};

struct User {};
struct Enemy {};
struct EnemyAI {};
struct WallWood {};
struct WallMetal {};

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

    world.set<Render>(render);
}

void initRand(flecs::world& world)
{
    Rand rand;
    rand.initGenerator();
    world.set<Rand>(rand);
}

void createEntities(flecs::world& world)
{
    world.entity().add<User>().add<sf::Sprite>();
    world.filter_builder<sf::Sprite, User, Render>()
            .term_at(3).singleton()
            .build()
            .each([](sf::Sprite& sprite, User, Render& render) {
                sprite.setTexture(render.userTexture);
                sprite.setPosition({400.f, 580.f});
            });

    for (int i = 0; i < MAX_ENEMY; i++)
        world.entity().add<Enemy>().add<sf::Sprite>();
    world.filter_builder<sf::Sprite, Enemy, Render, Rand>()
            .term_at(3).singleton()
            .term_at(4).singleton()
            .build()
            .each([](sf::Sprite& sprite, Enemy, Render& render, Rand& rand) {
                sprite.setTexture(render.enemyTexture);
                sf::Vector2f position;
                position.x = rand.getRandomFloat(SIZE_TANK / 2, WINDOW_WIDTH - SIZE_TANK / 2);
                position.y = rand.getRandomFloat(SIZE_TANK / 2, WINDOW_HEIGHT - SIZE_TANK / 2);
                sprite.setPosition(position);
            });

    for (int i = 0; i < MAX_ENEMY_AI; i++)
        world.entity().add<EnemyAI>().add<sf::Sprite>();
    world.filter_builder<sf::Sprite, EnemyAI, Render, Rand>()
            .term_at(3).singleton()
            .term_at(4).singleton()
            .build()
            .each([](sf::Sprite& sprite, EnemyAI, Render& render, Rand& rand) {
                sprite.setTexture(render.enemyAiTexture);
                sf::Vector2f position;
                position.x = rand.getRandomFloat(SIZE_TANK / 2, WINDOW_WIDTH - SIZE_TANK / 2);
                position.y = rand.getRandomFloat(SIZE_TANK / 2, WINDOW_HEIGHT - SIZE_TANK / 2);
                sprite.setPosition(position);
            });

    for (int i = 0; i < MAX_WALL_WOOD; i++)
        world.entity().add<WallWood>().add<sf::Sprite>();
    world.filter_builder<sf::Sprite, WallWood, Render, Rand>()
            .term_at(3).singleton()
            .term_at(4).singleton()
            .build()
            .each([](sf::Sprite& sprite, WallWood, Render& render, Rand& rand) {
                sprite.setTexture(render.wallWoodTexture_0);
                sf::Vector2f position;
                position.x = rand.getRandomFloat(SIZE_TANK / 2, WINDOW_WIDTH - SIZE_TANK / 2);
                position.y = rand.getRandomFloat(SIZE_TANK / 2, WINDOW_HEIGHT - SIZE_TANK / 2);
                sprite.setPosition(position);
            });

    for (int i = 0; i < MAX_WALL_METAL; i++)
        world.entity().add<WallMetal>().add<sf::Sprite>();
    world.filter_builder<sf::Sprite, WallMetal, Render, Rand>()
            .term_at(3).singleton()
            .term_at(4).singleton()
            .build()
            .each([](sf::Sprite& sprite, WallMetal, Render& render, Rand& rand) {
                sprite.setTexture(render.wallMetalTexture);
                sf::Vector2f position;
                position.x = rand.getRandomFloat(SIZE_TANK / 2, WINDOW_WIDTH - SIZE_TANK / 2);
                position.y = rand.getRandomFloat(SIZE_TANK / 2, WINDOW_HEIGHT - SIZE_TANK / 2);
                sprite.setPosition(position);
            });
}

int main(int argc, char *argv[])
{
    flecs::world world(argc, argv);
    initRender(world);
    initRand(world);
    createEntities(world);

    auto render = world.get<Render>();

    while(1)
    {
        render->window->clear();

        world.filter_builder<sf::Sprite, Render>()
                .term_at(2).singleton()
                .build()
                .each([](sf::Sprite& sprite, Render& render) {
                    render.window->draw(sprite);
                });

        render->window->display();
    }






//    while (window.isOpen())
//    {
//        pollEvents(window);
//        update();
//        redrawFrame(window);
//                window.clear();
//                window.draw();
//                window.display();
//    }
}