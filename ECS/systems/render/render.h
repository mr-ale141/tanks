#pragma ONCE
#include "../../init/utils.h"

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