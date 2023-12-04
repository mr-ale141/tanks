#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include "flecs.h"

constexpr unsigned WINDOW_WIDTH = 800;
constexpr unsigned WINDOW_HEIGHT = 600;
constexpr unsigned MAX_ENEMY = 10;
constexpr unsigned MAX_ENEMY_AI = 5;
constexpr unsigned MAX_WALL_WOOD = 50;
constexpr unsigned MAX_WALL_METAL = 10;

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

struct Textures {
    sf::Texture userTexture;
    sf::Texture enemyTexture;
    sf::Texture enemyAiTexture;
    sf::Texture userBulletTexture;
    sf::Texture enemyBulletTexture;
    sf::Texture wallMetalTexture;
    sf::Texture wallWoodTexture_0;
    sf::Texture wallWoodTexture_1;
    sf::Texture wallWoodTexture_2;
};

struct User {};
struct Enemy {};
struct EnemyAI {};
struct WallWood {};
struct WallMetal {};

void initTextures(flecs::world& world) {
    Textures textures;

    if (!textures.userTexture.loadFromFile("../sprites/user.png"))
    {
        std::cout << "Error: I can't read texture \"../sprites/user.png\"!!!\n";
        exit(1);
    }

    if (!textures.enemyTexture.loadFromFile("../sprites/enemy.png"))
    {
        std::cout << "Error: I can't read texture \"../sprites/enemy.png\"!!!\n";
        exit(1);
    }

    if (!textures.enemyAiTexture.loadFromFile("../sprites/enemy_ai.png"))
    {
        std::cout << "Error: I can't read texture \"../sprites/enemy_ai.png\"!!!\n";
        exit(1);
    }

    if (!textures.userBulletTexture.loadFromFile("../sprites/bullet_user.png"))
    {
        std::cout << "Error: I can't read texture \"../sprites/bullet_user.png\"!!!\n";
        exit(1);
    }

    if (!textures.enemyBulletTexture.loadFromFile("../sprites/bullet_enemy.png"))
    {
        std::cout << "Error: I can't read texture \"../sprites/bullet_enemy.png\"!!!\n";
        exit(1);
    }

    if (!textures.wallMetalTexture.loadFromFile("../sprites/wall_metal.png"))
    {
        std::cout << "Error: I can't read texture \"../sprites/wall_metal.png\"!!!\n";
        exit(1);
    }

    if (!textures.wallWoodTexture_0.loadFromFile("../sprites/wall_wood_0.png"))
    {
        std::cout << "Error: I can't read texture \"../sprites/wall_wood_0.png\"!!!\n";
        exit(1);
    }

    if (!textures.wallWoodTexture_1.loadFromFile("../sprites/wall_wood_1.png"))
    {
        std::cout << "Error: I can't read texture \"../sprites/wall_wood_1.png\"!!!\n";
        exit(1);
    }

    if (!textures.wallWoodTexture_2.loadFromFile("../sprites/wall_wood_2.png"))
    {
        std::cout << "Error: I can't read texture \"../sprites/wall_wood_2.png\"!!!\n";
        exit(1);
    }

    world.set<Textures>(textures);
}

void initRand(flecs::world& world)
{
    Rand rand;
    rand.initGenerator();
    world.set<Rand>(rand);
}

void createEntities(flecs::world& world)
{
    auto textures = world.entity<Textures>();

    world.entity().add<User>().add<sf::Sprite>().is_a(textures);
    world.each([](sf::Sprite& sprite, Textures& textures, User) {
        sprite.setTexture(textures.userTexture);
    });

    for (int i = 0; i < MAX_ENEMY; i++)
        world.entity().add<Enemy>().add<sf::Sprite>().is_a(textures);
    world.each([](sf::Sprite& sprite, Textures& textures, Enemy) {
        sprite.setTexture(textures.enemyTexture);
    });

    for (int i = 0; i < MAX_ENEMY_AI; i++)
        world.entity().add<EnemyAI>().add<sf::Sprite>().is_a(textures);
    world.each([](sf::Sprite& sprite, Textures& textures, EnemyAI) {
        sprite.setTexture(textures.enemyAiTexture);
    });

    for (int i = 0; i < MAX_ENEMY; i++)
        world.entity().add<WallWood>().add<sf::Sprite>().is_a(textures);
    world.each([](sf::Sprite& sprite, Textures& textures, WallWood) {
        sprite.setTexture(textures.wallWoodTexture_0);
    });

    for (int i = 0; i < MAX_ENEMY; i++)
        world.entity().add<WallMetal>().add<sf::Sprite>().is_a(textures);
    world.each([](sf::Sprite& sprite, Textures& textures, WallMetal) {
        sprite.setTexture(textures.wallMetalTexture);
    });
}

int main(int argc, char *argv[])
{
    flecs::world world(argc, argv);
    initTextures(world);
    initRand(world);
    createEntities(world);








//    sf::ContextSettings settings;
//    settings.antialiasingLevel = 8;
//    sf::RenderWindow window(
//            sf::VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT }),
//            "Arrow",
//            sf::Style::Default,
//            settings);



//    while (window.isOpen())
//    {
//        pollEvents(window);
//        update();
//        redrawFrame(window);
//    }
}