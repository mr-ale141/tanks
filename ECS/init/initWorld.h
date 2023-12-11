#pragma ONCE
#include "../systems/control/control.h"

void createEntities(flecs::world& world)
{
    world.entity().add<User>().add<sf::Sprite>().add<Moving>().add<Collisional>();

    for (int i = 0; i < MAX_ENEMY; i++)
        world.entity().add<Enemy>().add<sf::Sprite>().add<Moving>().add<Collisional>();

    for (int i = 0; i < MAX_ENEMY_AI; i++)
        world.entity().add<EnemyAI>().add<sf::Sprite>().add<Moving>().add<Collisional>();

    for (int i = 0; i < MAX_WALL_WOOD; i++)
        world.entity().add<WallWood>().add<sf::Sprite>();

    for (int i = 0; i < MAX_WALL_METAL; i++)
        world.entity().add<WallMetal>().add<sf::Sprite>();

    world.filter_builder<Moving, sf::Sprite, User, Render>()
            .term_at(4).singleton()
            .build()
            .each([](flecs::entity e, Moving& moving, sf::Sprite& sprite, User, Render& render) {
                moving.direction = UP;
                moving.preTimeMoving = render.clock.getElapsedTime().asSeconds();
                moving.positionScreen = NUM_POSITION_USER;
                render.busyPositionScreen[moving.positionScreen] = true;
                sprite.setTexture(render.userTexture);
                setScaleTank(sprite);
                setOriginCenter(sprite);
                sprite.setPosition(getPositionCenter(NUM_POSITION_USER));
            });

    world.filter_builder<Moving, sf::Sprite, Render, Rand>()
            .term<User>().oper(flecs::Not)
            .term_at(3).singleton()
            .term_at(4).singleton()
            .build()
            .each([](flecs::entity e, Moving& moving, sf::Sprite& sprite, Render& render, Rand& rand) {
                std::string typesStr = std::string (e.type().str());
                if (typesStr.find("EnemyAI") != std::string::npos)
                {
                    sprite.setTexture(render.enemyAiTexture);
                }
                else if (typesStr.find("Enemy") != std::string::npos)
                {
                    sprite.setTexture(render.enemyTexture);
                }
                moving.direction = directionEnum(rand.getRandomInt(0, 3));
                moving.preTimeMoving = render.clock.getElapsedTime().asSeconds();
                unsigned numPosition = getFreePosition(
                        rand,
                        render,
                        0,
                        MAX_POSITION_IN_SCREEN / 2 - 1);
                moving.positionScreen = numPosition;
                render.busyPositionScreen[numPosition] = true;
                sf::Vector2f position = getPositionCenter(numPosition);
                sprite.setPosition(position);
                setScaleTank(sprite);
                setOriginCenter(sprite);
                setDirection(sprite, moving.direction);
            });

    world.filter_builder<sf::Sprite, Render, Rand>()
            .term<Moving>().oper(flecs::Not)
            .term_at(2).singleton()
            .term_at(3).singleton()
            .build()
            .each([](flecs::entity e, sf::Sprite& sprite, Render& render, Rand& rand) {
                std::string typesStr = std::string (e.type().str());
                if (typesStr.find("WallWood") != std::string::npos)
                {
                    sprite.setTexture(render.wallWoodTexture_0);
                }
                else if (typesStr.find("WallMetal") != std::string::npos)
                {
                    sprite.setTexture(render.wallMetalTexture);
                }
                unsigned numPosition = getFreePosition(
                        rand,
                        render,
                        0,
                        MAX_POSITION_IN_SCREEN - 1);
                render.busyPositionScreen[numPosition] = true;
                sf::Vector2f position = getPositionCenter(numPosition);
                sprite.setPosition(position);
                setOriginCenter(sprite);
            });
}

void initWorld(flecs::world& world)
{
    initDrawSystem(world);
    initMovingSystems(world);
    createEntities(world);
    initControl(world);
    initCollisional(world);
}