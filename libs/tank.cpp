#include "headers/tank.hpp"

Tank::Tank(typeTank type, sf::Vector2f startPosition, sf::Vector2f startDirection, sf::Clock& clockRef)
{
    clock = clockRef;
    switch (type)
    {
    case userTank:
        if (!tankTexture.loadFromFile("./sprites/user.png"))
        {
            std::cout << "Error: I can't read texture \"./sprites/user.png\"!!!\n";
            exit(1);
        }
        speed = SPEED_USER;
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

    position = startPosition;
    tank.setPosition(startPosition);

    size = tank.getLocalBounds();
    tank.setOrigin({size.width / 2, size.height / 2});

    countHit = 0;

    preTime = clock.getElapsedTime().asSeconds();

    setDirection(startDirection);
}

void Tank::updatePosition()
{
    float currentTime = clock.getElapsedTime().asSeconds();
    float deltaTime = currentTime - preTime;
    float offset = speed * deltaTime;
    position += direction * offset;
    tank.setPosition(position);
    preTime = currentTime;
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