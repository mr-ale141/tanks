#include "headers/tank.hpp"

Tank::Tank(typeTank startType, sf::Vector2f startPosition, sf::Vector2f startDirection, sf::Clock& clockRef)
{
    clock = clockRef;
    type = startType;
    setTankParameters();
    setScaleTank();
    setOriginCenter();
    setPosition(startPosition);
    countHit = 0;
    timeLastShoot = 0.f;
    isDamaged = false;
    setDirection(startDirection);
    preTimeUpdatePosition = clock.getElapsedTime().asSeconds();
    preTimeUpdateDirection = clock.getElapsedTime().asSeconds();
}

void Tank::updatePosition()
{
    float currentTime = clock.getElapsedTime().asSeconds();
    float deltaTime = currentTime - preTimeUpdatePosition;
    float offset = speed * deltaTime;
    sf::Vector2f newPosition = position + direction * offset;
    setPosition(newPosition);
    preTimeUpdatePosition = currentTime;
}

void Tank::setPosition(sf::Vector2f newPosition)
{
    tank.setPosition(position = newPosition);
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
    target.draw(tank, states);
}

void Tank::setTankParameters()
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
        speed = 0.f;
        shootSpeed = SHOOT_SPEED_ENEMY;
        break;
    case enemyTankAI:
        if (!tankTexture.loadFromFile("./sprites/enemy_ai.png"))
        {
            std::cout << "Error: I can't read texture \"./sprites/enemy_ai.png\"!!!\n";
            exit(1);
        }
        speed = 0.f;
        shootSpeed = SHOOT_SPEED_ENEMY_AI;
        break;
    default:
        std::cout << "Error: Unknow typeTank!!!\n";
        exit(1);
    }
    tank.setTexture(tankTexture);
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
    switch (type)
    {
        case userTank:
            speed = SPEED_USER;
            break;
        case enemyTank:
            speed = SPEED_ENEMY;
            break;
        case enemyTankAI:
            speed = SPEED_ENEMY_AI;
            break;
        default:
            break;
    }
}

void Tank::shoot(World& world)
{
    float currTime = clock.getElapsedTime().asSeconds();
    bool iSee = false;
    if (type == userTank)
    {
        if (currTime - timeLastShoot > 1 / SHOOT_SPEED_USER)
        {
            world.createBullet(position, direction, false);
            timeLastShoot = currTime;
        }
    }
    else
    {
        sf::Vector2f positionUser = world.user->getPosition();
        sf::Vector2f directionForUser = positionUser - position;
        float directionForUserModule = world.getModule(directionForUser);
        sf::Vector2f directionForUserNormal = {
            directionForUser.x / directionForUserModule,
            directionForUser.y / directionForUserModule
        };
        float scalar = directionForUserNormal.x * direction.x + directionForUserNormal.y * direction.y;
        if (std::abs(directionForUser.x) < SIZE_TANK / 2 || std::abs(directionForUser.y) < SIZE_TANK / 2)
            if (scalar > 0.9)
                iSee = true;
    }
    if (iSee && type == enemyTank && currTime - timeLastShoot > 1 / SHOOT_SPEED_ENEMY)
    {
        world.createBullet(position, direction, true);
        timeLastShoot = currTime;
    }
    currTime = clock.getElapsedTime().asSeconds();
    if (iSee && type == enemyTankAI && currTime - timeLastShoot > 1 / SHOOT_SPEED_ENEMY_AI)
    {
        world.createBullet(position, direction, true);
        timeLastShoot = currTime;
    }
}

void Tank::destroy()
{
    stop();
    isDamaged = true;
}

void Tank::update(World& world)
{
    if (!isDamaged)
    {
        updatePosition();
        if (type != userTank) shoot(world);

    }
}
