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
    world.createBullet(position, direction, type != userTank);
    timeLastShoot = clock.getElapsedTime().asSeconds();
}

void Tank::update()
{
    updatePosition();
}
