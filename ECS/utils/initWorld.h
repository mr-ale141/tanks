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

const float SPEED_USER = 80.f;
const float SPEED_ENEMY_AI = 20.f;
const float SPEED_ENEMY = 40.f;
const float SHOOT_SPEED_USER = 2.f;
const float SHOOT_SPEED_ENEMY = 1.f / 3.f;
const float SHOOT_SPEED_ENEMY_AI = 1.f / 4.f;
const int HP_USER = 3;
const int HP_ENEMY = 1;
const int HP_ENEMY_AI = 2;

constexpr unsigned ROW_COUNT = int(WINDOW_HEIGHT / SIZE_TANK);
constexpr unsigned COLUMN_COUNT = int(WINDOW_WIDTH / SIZE_TANK);
constexpr unsigned MAX_POSITION_IN_SCREEN = COLUMN_COUNT * ROW_COUNT;
constexpr unsigned NUM_POSITION_USER = (ROW_COUNT - 1) * COLUMN_COUNT + COLUMN_COUNT / 2 - 1;
const sf::Vector2f POSITION_USER = {
        float(SIZE_TANK) / 2 + NUM_POSITION_USER % COLUMN_COUNT * SIZE_TANK,
        float(SIZE_TANK) / 2 + NUM_POSITION_USER / COLUMN_COUNT * SIZE_TANK};
const sf::Vector2f DIRECTIONS[4] = {
        { 0.f, -1.f },
        { 1.f, 0.f },
        { 0.f, 1.f },
        { -1.f, 0.f }
};

enum directionEnum
{
    UP,
    RIGHT,
    DOWN,
    LEFT
};

struct Moving {
    directionEnum direction;
    float speed;
    float preTimeDirection;
    float preTimeMoving;
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
    bool busyPositionScreen[MAX_POSITION_IN_SCREEN];
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

    for (bool &i : render.busyPositionScreen)
        i = false;

    render.busyPositionScreen[NUM_POSITION_USER] = true;

    world.set<Render>(render);
}

void initRand(flecs::world& world)
{
    Rand rand;
    rand.initGenerator();
    world.set<Rand>(rand);
}

void setScaleTank(sf::Sprite& sprite)
{
    sf::Vector2f targetSize;
    sf::FloatRect size = sprite.getGlobalBounds();
    float ratio = size.width / size.height;
    if (size.height > size.width)
        targetSize = { ratio * SIZE_TANK, SIZE_TANK };
    else
        targetSize = { SIZE_TANK, SIZE_TANK / ratio };
    sprite.setScale(
            targetSize.x / size.width,
            targetSize.y / size.height);
}

void setOriginCenter(sf::Sprite& sprite)
{
    sf::FloatRect size = sprite.getLocalBounds();
    sprite.setOrigin({ size.width / 2, size.height / 2 });
}

void setRandomPosition(sf::Sprite& sprite, Rand& rand, Render& render, int minNumPosition, int maxNumPosition)
{
    sf::Vector2f position;
    const int MAX_TRYING = 500;
    unsigned numPosition;
    int countTrying = 0;
    do
    {
        countTrying++;
        numPosition = rand.getRandomInt(minNumPosition, maxNumPosition);
        if (numPosition == NUM_POSITION_USER)
            continue;
    } while (render.busyPositionScreen[numPosition] && countTrying < MAX_TRYING);
    if (countTrying == MAX_TRYING)
        std::cout << "All position is BUSY!!!\n";
    else
        render.busyPositionScreen[numPosition] = true;
    unsigned rowNumber = numPosition / COLUMN_COUNT;
    unsigned columnNumber = numPosition % COLUMN_COUNT;
    position.x = float(SIZE_TANK) / 2 + float(columnNumber) * SIZE_TANK;
    position.y = float(SIZE_TANK) / 2 + float(rowNumber) * SIZE_TANK;
    sprite.setPosition(position);
}

void setDirection(sf::Sprite& sprite, directionEnum direction)
{
    if (direction == UP)
        sprite.setRotation(0.f);
    else if (direction == RIGHT)
        sprite.setRotation(90.f);
    else if (direction == DOWN)
        sprite.setRotation(180.f);
    else if (direction == LEFT)
        sprite.setRotation(270.f);
    else
        std::cout << "Error: Unknown direction!!!\n";
}

void createEntities(flecs::world& world)
{
    world.entity().add<User>().add<sf::Sprite>().add<Moving>();
    world.filter_builder<Moving, sf::Sprite, User, Render>()
            .term_at(4).singleton()
            .build()
            .each([](Moving& moving, sf::Sprite& sprite, User, Render& render) {
                moving.direction = UP;
                moving.speed = 0.f;
                moving.preTimeDirection = render.clock.getElapsedTime().asSeconds();
                moving.preTimeMoving = render.clock.getElapsedTime().asSeconds();
                sprite.setTexture(render.userTexture);
                setScaleTank(sprite);
                setOriginCenter(sprite);
                sprite.setPosition(POSITION_USER);
            });

    for (int i = 0; i < MAX_ENEMY; i++)
        world.entity().add<Enemy>().add<sf::Sprite>().add<Moving>();
    world.filter_builder<Moving, sf::Sprite, Enemy, Render, Rand>()
            .term_at(4).singleton()
            .term_at(5).singleton()
            .build()
            .each([](Moving& moving, sf::Sprite& sprite, Enemy, Render& render, Rand& rand) {
                moving.direction = directionEnum(rand.getRandomInt(0, 3));
                moving.speed = 0.f;
                moving.preTimeDirection = render.clock.getElapsedTime().asSeconds();
                moving.preTimeMoving = render.clock.getElapsedTime().asSeconds();
                sprite.setTexture(render.enemyTexture);
                setRandomPosition(
                        sprite,
                        rand,
                        render,
                        0,
                        MAX_POSITION_IN_SCREEN / 2 - 1);
                setScaleTank(sprite);
                setOriginCenter(sprite);
                setDirection(sprite, moving.direction);
            });

    for (int i = 0; i < MAX_ENEMY_AI; i++)
        world.entity().add<EnemyAI>().add<sf::Sprite>().add<Moving>();
    world.filter_builder<Moving, sf::Sprite, EnemyAI, Render, Rand>()
            .term_at(4).singleton()
            .term_at(5).singleton()
            .build()
            .each([](Moving& moving, sf::Sprite& sprite, EnemyAI, Render& render, Rand& rand) {
                moving.direction = directionEnum(rand.getRandomInt(0, 3));
                moving.speed = 0.f;
                moving.preTimeDirection = render.clock.getElapsedTime().asSeconds();
                moving.preTimeMoving = render.clock.getElapsedTime().asSeconds();
                sprite.setTexture(render.enemyAiTexture);
                setRandomPosition(
                        sprite,
                        rand,
                        render,
                        0,
                        MAX_POSITION_IN_SCREEN / 2 - 1);
                setScaleTank(sprite);
                setOriginCenter(sprite);
                setDirection(sprite, moving.direction);
            });

    for (int i = 0; i < MAX_WALL_WOOD; i++)
        world.entity().add<WallWood>().add<sf::Sprite>();
    world.filter_builder<sf::Sprite, WallWood, Render, Rand>()
            .term_at(3).singleton()
            .term_at(4).singleton()
            .build()
            .each([](sf::Sprite& sprite, WallWood, Render& render, Rand& rand) {
                sprite.setTexture(render.wallWoodTexture_0);
                setRandomPosition(
                        sprite,
                        rand,
                        render,
                        0,
                        MAX_POSITION_IN_SCREEN - 1);
                setScaleTank(sprite);
                setOriginCenter(sprite);
            });

    for (int i = 0; i < MAX_WALL_METAL; i++)
        world.entity().add<WallMetal>().add<sf::Sprite>();
    world.filter_builder<sf::Sprite, WallMetal, Render, Rand>()
            .term_at(3).singleton()
            .term_at(4).singleton()
            .build()
            .each([](sf::Sprite& sprite, WallMetal, Render& render, Rand& rand) {
                sprite.setTexture(render.wallMetalTexture);
                setRandomPosition(
                        sprite,
                        rand,
                        render,
                        0,
                        MAX_POSITION_IN_SCREEN - 1);
                setScaleTank(sprite);
                setOriginCenter(sprite);
            });
}

void initWorld(flecs::world& world)
{
    initRender(world);
    initRand(world);
    createEntities(world);
}