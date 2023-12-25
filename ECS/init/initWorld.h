#pragma ONCE
#include "../systems/control/control.h"

void createUser(flecs::world& world)
{
    User user = {};
    sf::Sprite sprite;
    Moving moving = {};
    Collisional collisional = {};
    Live live = {};
    live.hp = HP_USER;
    auto render = world.get_mut<Render>();

    user.nextTimeShoot = 0.f;
    moving.direction = UP;
    moving.speed = 0.f;
    moving.nextTimeDirection = 0.f;
    moving.preTimeMoving = render->clock.getElapsedTime().asSeconds();
    moving.numPositionScreen = NUM_POSITION_USER;
    collisional.iCantMove = true;
    render->busyPositionScreen[moving.numPositionScreen] = true;
    sprite.setTexture(render->userTexture);
    setScaleTank(sprite);
    setOriginCenter(sprite);
    sprite.setPosition(getPositionCenter(NUM_POSITION_USER));
    setDirection(sprite, moving.direction);

    world.entity("User").set<User>(user).set<sf::Sprite>(sprite).set<Moving>(moving).set<Collisional>(collisional).set<Live>(live);
}

void createBaseUser(flecs::world& world)
{
    sf::Sprite sprite;
    Live live = {};
    live.hp = HP_BASE;
    auto render = world.get_mut<Render>();
    render->busyPositionScreen[NUM_POSITION_BASE_USER] = true;
    sprite.setTexture(render->baseUser);
    setOriginCenter(sprite);
    sprite.setPosition(getPositionCenter(NUM_POSITION_BASE_USER));

    world.entity().add<BaseUser>().set<sf::Sprite>(sprite).set<Live>(live);
}

void createBaseEnemy(flecs::world& world)
{
    sf::Sprite sprite;
    Live live = {};
    live.hp = HP_BASE;
    auto render = world.get_mut<Render>();
    render->busyPositionScreen[NUM_POSITION_BASE_ENEMY] = true;
    sprite.setTexture(render->baseEnemy);
    setOriginCenter(sprite);
    sprite.setPosition(getPositionCenter(NUM_POSITION_BASE_ENEMY));

    world.entity().add<BaseEnemy>().set<sf::Sprite>(sprite).set<Live>(live);
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
        Live live = {};
        live.hp = HP_ENEMY;
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
        moving.numPositionScreen = numPosition;
        render->busyPositionScreen[moving.numPositionScreen] = true;
        setDirection(sprite, moving.direction);
        sprite.setTexture(render->enemyTexture);
        sf::Vector2f position = getPositionCenter(moving.numPositionScreen);
        sprite.setPosition(position);
        setScaleTank(sprite);
        setOriginCenter(sprite);
        world.entity().set<Enemy>(enemy).set<sf::Sprite>(sprite).set<Moving>(moving).set<Collisional>(collisional).set<Live>(live);
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
        Live live = {};
        live.hp = HP_ENEMY_AI;
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
        moving.numPositionScreen = numPosition;
        render->busyPositionScreen[moving.numPositionScreen] = true;
        setDirection(sprite, moving.direction);
        sprite.setTexture(render->enemyAiTexture);
        sf::Vector2f position = getPositionCenter(moving.numPositionScreen);
        setScaleTank(sprite);
        setOriginCenter(sprite);
        sprite.setPosition(position);
        world.entity().set<EnemyAI>(enemyAI).set<sf::Sprite>(sprite).set<Moving>(moving).set<Collisional>(collisional).set<Live>(live);
    }
}

void createWallWoods(flecs::world& world)
{
    auto render = world.get_mut<Render>();
    auto rand = world.get_mut<Rand>();
    const int HP_WALL_WOOD = 3;
    for (int i = 0; i < MAX_POSITION_IN_SCREEN; i++)
    {
        Live live = {};
        live.hp = HP_WALL_WOOD;
        sf::Sprite sprite;
        sprite.setTexture(render->wallWoodTexture_0);
        unsigned numPosition = wallWoodMaps[LEVEL][i];
        render->busyPositionScreen[numPosition] = true;
        sf::Vector2f position = getPositionCenter(numPosition);
        sprite.setPosition(position);
        setOriginCenter(sprite);
        world.entity().add<WallWood>().set<sf::Sprite>(sprite).set<Live>(live);
    }
}

void createWallMetal(flecs::world& world)
{
    auto render = world.get_mut<Render>();
    auto rand = world.get_mut<Rand>();
    for (int i = 0; i < MAX_POSITION_IN_SCREEN; i++)
    {
        sf::Sprite sprite;
        sprite.setTexture(render->wallMetalTexture);
        unsigned numPosition = wallMetalMaps[LEVEL][i];
        render->busyPositionScreen[numPosition] = true;
        sf::Vector2f position = getPositionCenter(numPosition);
        sprite.setPosition(position);
        setOriginCenter(sprite);
        world.entity().add<WallMetal>().set<sf::Sprite>(sprite);
    }
}

void createEntities(flecs::world& world)
{
    createWallWoods(world);
    createWallMetal(world);
    createUser(world);
    createBaseUser(world);
    createBaseEnemy(world);
    createEnemies(world);
    createEnemiesAI(world);
}

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

    if (!render.baseUser.loadFromFile("../sprites/base.png"))
    {
        std::cout << "Error: I can't read texture \"../sprites/base.png\"!!!\n";
        exit(1);
    }

    if (!render.baseEnemy.loadFromFile("../sprites/baseEnemy.png"))
    {
        std::cout << "Error: I can't read texture \"../sprites/baseEnemy.png\"!!!\n";
        exit(1);
    }

    for (int i = 0; i < COUNT_COLUMN_TEXTURE_FIRE; i++)
    {
        sf::IntRect positionInTexture = {
                i * SIZE_TEXTURE_FIRE,
                0,
                SIZE_TEXTURE_FIRE,
                SIZE_TEXTURE_FIRE
        };
        if (!render.fire[i].loadFromFile("../sprites/fire.png", positionInTexture))
        {
            std::cout << "Error: I can't read texture \"../sprites/fire.png\"!!!\n";
            exit(1);
        }
    }

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    render.window = new sf::RenderWindow;
    render.window->create(sf::VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT }),
                          "Tanks",
                          sf::Style::Default,
                          settings);

    for (auto &i : render.busyPositionScreen)
        i = false;

    world.set<Render>(render);
}

void initRand(flecs::world& world)
{
    Rand rand;
    rand.initGenerator();
    world.set<Rand>(rand);
}

void initSingletons(flecs::world& world)
{
    initRender(world);
    initRand(world);
}

void initWorld(flecs::world& world)
{
    MAX_ENEMY = MAX_ENEMY_DEFAULT + LEVEL;
    MAX_ENEMY_AI = MAX_ENEMY_AI_DEFAULT + LEVEL;

    SPEED_USER = SPEED_USER_DEFAULT + float(LEVEL) * 2.f;
    SPEED_ENEMY_AI = SPEED_ENEMY_AI_DEFAULT + float(LEVEL) * 2.f;
    SPEED_ENEMY = SPEED_ENEMY_DEFAULT + float(LEVEL) * 2.f;

    SHOOT_SPEED_USER = SHOOT_SPEED_USER_DEFAULT + float(LEVEL) / 2.f;
    SHOOT_SPEED_ENEMY = SHOOT_SPEED_ENEMY_DEFAULT + float(LEVEL) / 4.f;
    SHOOT_SPEED_ENEMY_AI = SHOOT_SPEED_ENEMY_AI_DEFAULT + float(LEVEL) / 9.f;

    SPEED_BULLET = SPEED_BULLET_DEFAULT + float(LEVEL) * 10;

    initSingletons(world);
    initRenderSystems(world);
    initMovingSystems(world);
    createEntities(world);
    initControlSystems(world);
    initCollisionalSystems(world);
}