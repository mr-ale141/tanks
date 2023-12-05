#pragma ONCE
#include "../systems/control/control.h"

constexpr unsigned MAX_ENEMY = 10;
constexpr unsigned MAX_ENEMY_AI = 5;
constexpr unsigned MAX_WALL_WOOD = 50;
constexpr unsigned MAX_WALL_METAL = 10;

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

unsigned getFreePosition(Rand& rand, Render& render, int minNumPosition, int maxNumPosition)
{
    const int MAX_TRYING = 500;
    unsigned numPosition = 0;
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
    return numPosition;
}

void createEntities(flecs::world& world)
{
    world.entity().add<User>().add<sf::Sprite>().add<Moving>();

    for (int i = 0; i < MAX_ENEMY; i++)
        world.entity().add<Enemy>().add<sf::Sprite>().add<Moving>();

    for (int i = 0; i < MAX_ENEMY_AI; i++)
        world.entity().add<EnemyAI>().add<sf::Sprite>().add<Moving>();

    for (int i = 0; i < MAX_WALL_WOOD; i++)
        world.entity().add<WallWood>().add<sf::Sprite>();

    for (int i = 0; i < MAX_WALL_METAL; i++)
        world.entity().add<WallMetal>().add<sf::Sprite>();



    world.filter_builder<Moving, sf::Sprite, User, Render>()
            .term_at(4).singleton()
            .build()
            .each([](flecs::entity e, Moving& moving, sf::Sprite& sprite, User, Render& render) {
                moving.direction = UP;
                moving.speed = 0.f;
                moving.preTimeMoving = render.clock.getElapsedTime().asSeconds();
                moving.positionScreen = NUM_POSITION_USER;
                render.busyPositionScreen[moving.positionScreen] = flecs::id(e.id());
                sprite.setTexture(render.userTexture);
                setScaleTank(sprite);
                setOriginCenter(sprite);
                sprite.setPosition(POSITION_USER);
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
                moving.speed = 0.f;
                moving.preTimeMoving = render.clock.getElapsedTime().asSeconds();
                unsigned numPosition = getFreePosition(
                        rand,
                        render,
                        0,
                        MAX_POSITION_IN_SCREEN / 2 - 1);
                moving.positionScreen = numPosition;
                render.busyPositionScreen[numPosition] = flecs::id(e.id());
                sf::Vector2f position;
                unsigned rowNumber = numPosition / COLUMN_COUNT;
                unsigned columnNumber = numPosition % COLUMN_COUNT;
                position.x = float(SIZE_TANK) / 2 + float(columnNumber) * SIZE_TANK;
                position.y = float(SIZE_TANK) / 2 + float(rowNumber) * SIZE_TANK;
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
                render.busyPositionScreen[numPosition] = flecs::id(e.id());
                sf::Vector2f position;
                unsigned rowNumber = numPosition / COLUMN_COUNT;
                unsigned columnNumber = numPosition % COLUMN_COUNT;
                position.x = float(SIZE_TANK) / 2 + float(columnNumber) * SIZE_TANK;
                position.y = float(SIZE_TANK) / 2 + float(rowNumber) * SIZE_TANK;
                sprite.setPosition(position);
                setOriginCenter(sprite);
            });
}

void initWorld(flecs::world& world)
{
    initDrawSystem(world);
    initMovingSystem(world);
    createEntities(world);
    initControlObserver(world);
}