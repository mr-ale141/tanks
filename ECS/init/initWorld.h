#pragma ONCE
#include "../systems/control/control.h"

void createUser(flecs::world& world)
{
    User user = {};
    sf::Sprite sprite;
    Moving moving = {};
    Collisional collisional = {};
    auto render = world.get_mut<Render>();

    user.hp = HP_USER;
    user.nextTimeShoot = 0.f;
    moving.direction = UP;
    moving.speed = 0.f;
    moving.nextTimeDirection = 0.f;
    moving.preTimeMoving = render->clock.getElapsedTime().asSeconds();
    moving.positionScreen = NUM_POSITION_USER;
    collisional.iCantMove = true;
    render->busyPositionScreen[moving.positionScreen] = true;
    sprite.setTexture(render->userTexture);
    setScaleTank(sprite);
    setOriginCenter(sprite);
    sprite.setPosition(getPositionCenter(NUM_POSITION_USER));
    setDirection(sprite, moving.direction);

    world.entity().set<User>(user).set<sf::Sprite>(sprite).set<Moving>(moving).set<Collisional>(collisional);
}

void createEnemies(flecs::world& world)
{
    auto render = world.get_mut<Render>();
    auto rand = world.get_mut<Rand>();
    for (int i = 0; i < MAX_ENEMY; i++)
    {
        Enemy enemy = {};
        sf::Sprite sprite;
        Moving moving = {};
        Collisional collisional = {};
        enemy.hp = HP_ENEMY;
        enemy.nextTimeShoot = 0.f;
        moving.speed = 0.f;
        moving.nextTimeDirection = 0.f;
        moving.preTimeMoving = render->clock.getElapsedTime().asSeconds();
        collisional.iCantMove = true;
        moving.direction = directionEnum(rand->getRandomInt(0, 3));
        unsigned numPosition = getFreePosition(
                *rand,
                *render,
                0,
                MAX_POSITION_IN_SCREEN / 2 - 1);
        moving.positionScreen = numPosition;
        render->busyPositionScreen[moving.positionScreen] = true;
        setDirection(sprite, moving.direction);
        sprite.setTexture(render->enemyTexture);
        sf::Vector2f position = getPositionCenter(moving.positionScreen);
        sprite.setPosition(position);
        setScaleTank(sprite);
        setOriginCenter(sprite);
        world.entity().set<Enemy>(enemy).set<sf::Sprite>(sprite).set<Moving>(moving).set<Collisional>(collisional);
    }
}

void createEnemiesAI(flecs::world& world)
{
    auto render = world.get_mut<Render>();
    auto rand = world.get_mut<Rand>();
    for (int i = 0; i < MAX_ENEMY_AI; i++)
    {
        EnemyAI enemyAI = {};
        sf::Sprite sprite;
        Moving moving = {};
        Collisional collisional = {};
        enemyAI.hp = HP_ENEMY_AI;
        enemyAI.nextTimeShoot = 0.f;
        moving.speed = 0.f;
        moving.nextTimeDirection = 0.f;
        moving.preTimeMoving = render->clock.getElapsedTime().asSeconds();
        collisional.iCantMove = true;
        moving.direction = directionEnum(rand->getRandomInt(0, 3));
        unsigned numPosition = getFreePosition(
                *rand,
                *render,
                0,
                MAX_POSITION_IN_SCREEN / 2 - 1);
        moving.positionScreen = numPosition;
        render->busyPositionScreen[moving.positionScreen] = true;
        setDirection(sprite, moving.direction);
        sprite.setTexture(render->enemyAiTexture);
        sf::Vector2f position = getPositionCenter(moving.positionScreen);
        setScaleTank(sprite);
        setOriginCenter(sprite);
        sprite.setPosition(position);
        world.entity().set<EnemyAI>(enemyAI).set<sf::Sprite>(sprite).set<Moving>(moving).set<Collisional>(collisional);
    }
}

void createWallWoods(flecs::world& world)
{
    auto render = world.get_mut<Render>();
    auto rand = world.get_mut<Rand>();
    for (int i = 0; i < MAX_WALL_WOOD; i++)
    {
        sf::Sprite sprite;
        sprite.setTexture(render->wallWoodTexture_0);
        unsigned numPosition = getFreePosition(
                *rand,
                *render,
                0,
                MAX_POSITION_IN_SCREEN - 1);
        render->busyPositionScreen[numPosition] = true;
        sf::Vector2f position = getPositionCenter(numPosition);
        sprite.setPosition(position);
        setOriginCenter(sprite);
        world.entity().add<WallWood>().set<sf::Sprite>(sprite);
    }
}

void createWallMetal(flecs::world& world)
{
    auto render = world.get_mut<Render>();
    auto rand = world.get_mut<Rand>();
    for (int i = 0; i < MAX_WALL_METAL; i++)
    {
        sf::Sprite sprite;
        sprite.setTexture(render->wallMetalTexture);
        unsigned numPosition = getFreePosition(
                *rand,
                *render,
                0,
                MAX_POSITION_IN_SCREEN - 1);
        render->busyPositionScreen[numPosition] = true;
        sf::Vector2f position = getPositionCenter(numPosition);
        sprite.setPosition(position);
        setOriginCenter(sprite);
        world.entity().add<WallMetal>().set<sf::Sprite>(sprite);
    }
}

void createEntities(flecs::world& world)
{
    createUser(world);
    createEnemies(world);
    createEnemiesAI(world);
    createWallWoods(world);
    createWallMetal(world);
}

void initWorld(flecs::world& world)
{
    initDrawSystem(world);
    initMovingSystems(world);
    createEntities(world);
    initControl(world);
    initCollisional(world);
}