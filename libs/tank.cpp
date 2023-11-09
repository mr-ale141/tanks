#include "headers/tank.hpp"
#include "headers/world.hpp"

Tank::Tank(typeTank startType, sf::Vector2f startPosition, sf::Vector2f startDirection, sf::Clock& clockRef)
{
    clock = clockRef;
    type = startType;
    setTankParametrs(type);
    setScaleTank();
    setOriginCenter();
    setPosition(startPosition);
    countHit = 0;
    timeLastShoot = 0.f;
    isDamaged = false;
    setDirection(startDirection);
    preTime = clock.getElapsedTime().asSeconds();
}

void Tank::updatePosition()
{
    float currentTime = clock.getElapsedTime().asSeconds();
    float deltaTime = currentTime - preTime;
    float offset = speed * deltaTime;
    sf::Vector2f newPosition = position + direction * offset;
    setPosition(newPosition);
    preTime = currentTime;
}

void Tank::setPosition(sf::Vector2f newPosition)
{
    position = newPosition;
    tank.setPosition(newPosition);
}

sf::Vector2f Tank::getDirection()
{
    return direction;
}

sf::Vector2f Tank::getPosition()
{
    return position;
}

void Tank::setDirection(sf::Vector2f newDirection)
{
    direction = newDirection;
    if (direction == DIRECTIONS[UP])
        tank.setRotation(0.f);
    else if (direction == DIRECTIONS[RIGHT])
        tank.setRotation(90.f);
    else if (direction == DIRECTIONS[DOWN])
        tank.setRotation(180.f);
    else if (direction == DIRECTIONS[LEFT])
        tank.setRotation(270.f);
    else
        std::cout << "Error: Unknow direction!!!\n";
}

void Tank::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (isDamaged)
    {
        target.draw(tank, states);
        target.draw(fire, states);
    }
    else
        target.draw(tank, states);
}

void Tank::setTankParametrs(typeTank type)
{
    switch (type)
    {
    case userTank:
        if (!tankTexture.loadFromFile("./sprites/user.png"))
        {
            std::cout << "Error: I can't read texture \"./sprites/user.png\"!!!\n";
            exit(1);
        }
        speed = 0.f;
        shootSpeed = SHOOT_SPEED_USER;
        break;
    case enemyTank:
        if (!tankTexture.loadFromFile("./sprites/enemy.png"))
        {
            std::cout << "Error: I can't read texture \"./sprites/enemy.png\"!!!\n";
            exit(1);
        }
        speed = SPEED_ENEMY;
        shootSpeed = SHOOT_SPEED_ENEMY;
        break;
    case enemyTankAI:
        if (!tankTexture.loadFromFile("./sprites/enemy_ai.png"))
        {
            std::cout << "Error: I can't read texture \"./sprites/enemy_ai.png\"!!!\n";
            exit(1);
        }
        speed = SPEED_ENEMY_AI;
        shootSpeed = SHOOT_SPEED_ENEMY_AI;
        break;
    default:
        std::cout << "Error: Unknow typeTank!!!\n";
        exit(1);
    }
    tank.setTexture(tankTexture);
    if (!fireTexture.create(SIZE_TEXTURE_FIRE, SIZE_TEXTURE_FIRE))
    {
        std::cout << "Error: I can't create texture 'fire'!!!\n";
        exit(1);
    }
    fire.setTexture(fireTexture);
}

void Tank::setScaleTank()
{
    sf::Vector2f targetSize;
    sf::FloatRect size = tank.getGlobalBounds();
    float ratio = size.width / size.height;
    if (size.height > size.width)
        targetSize = { ratio * SIZE_TANK, SIZE_TANK };
    else
        targetSize = { SIZE_TANK, SIZE_TANK / ratio };
    tank.setScale(
        targetSize.x / size.width,
        targetSize.y / size.height);
}

void Tank::setOriginCenter()
{
    sf::FloatRect size = tank.getLocalBounds();
    tank.setOrigin({ size.width / 2, size.height / 2 });
}

float Tank::getSpeed()
{
    return speed;
}

void Tank::stop()
{
    speed = 0.f;
}

void Tank::drive()
{
    speed = SPEED_USER;
}

void Tank::shoot(World& world)
{
    float currTime = clock.getElapsedTime().asSeconds();
    if (currTime - timeLastShoot > 1 / SHOOT_SPEED_USER)
    {
        world.createBullet(position, direction, type != userTank);
        timeLastShoot = clock.getElapsedTime().asSeconds();
    }
}

sf::IntRect Tank::getPositionInTexture()
{
    return sf::IntRect(
        currColumnTextureFire * SIZE_TEXTURE_FIRE,
        currRowTextureFire * SIZE_TEXTURE_FIRE,
        SIZE_TEXTURE_FIRE,
        SIZE_TEXTURE_FIRE
    );
}

void Tank::updateFireTexture()
{
    if (!fireTexture.loadFromFile("./sprites/fire.png", getPositionInTexture()))
    {
        std::cout << "Error: I can't read texture \"./sprites/fire.png\"!!!\n";
        exit(1);
    }
}

void Tank::updateFire()
{
    float currTime = clock.getElapsedTime().asSeconds();
    float dt = currTime - lastTimeUpdateFire;
    if (dt > STEP_UPDATE_FIRE)
    {
        if (currColumnTextureFire++ > MAX_COLUNN_TEXTURE_FIRE)
            currColumnTextureFire = 0;
        if (currRowTextureFire++ > MAX_ROW_TEXTURE_FIRE)
            currRowTextureFire = 0;
        updateFireTexture();
        fire.setTexture(fireTexture);
        lastTimeUpdateFire = clock.getElapsedTime().asSeconds();
    }
}

void Tank::destroy()
{
    stop();
    currColumnTextureFire = 0;
    currRowTextureFire = 0;
    updateFireTexture();
    fire.setTexture(fireTexture);
    fire.setPosition(position);
    fire.setOrigin({ SIZE_TEXTURE_FIRE / 2, SIZE_TEXTURE_FIRE / 2});
    lastTimeUpdateFire = clock.getElapsedTime().asSeconds();
    isDamaged = true;
}

void Tank::update()
{
    if (isDamaged)
        updateFire();
    else
        updatePosition();
}
