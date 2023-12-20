#pragma ONCE
#include "components.h"

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

void setOriginUpCenter(sf::Sprite& sprite)
{
    sf::FloatRect size = sprite.getGlobalBounds();
    sprite.setOrigin({ size.width / 2, 0 });
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

float getModule(sf::Vector2f v)
{
    float module = std::sqrt(v.x * v.x + v.y * v.y);
    return module;
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

sf::Vector2f getPositionCenter(unsigned numPosition)
{
    sf::Vector2f position;
    unsigned rowNumber = numPosition / COLUMN_COUNT;
    unsigned columnNumber = numPosition % COLUMN_COUNT;
    position.x = float(SIZE_TANK) / 2 + float(columnNumber) * SIZE_TANK;
    position.y = float(SIZE_TANK) / 2 + float(rowNumber) * SIZE_TANK;
    return position;
}

unsigned getNumPosition(sf::Vector2f position)
{
    auto column = unsigned(position.x / float(SIZE_TANK));
    auto row = unsigned(position.y / float(SIZE_TANK));
    unsigned numPosition = row * COLUMN_COUNT + column;
    return numPosition;
}

bool checkBarriers(const Render* render, directionEnum directionEnemy, unsigned numPositionEnemy, unsigned numPositionUser)
{
    unsigned checkNum = numPositionEnemy;
    do {
        switch (directionEnemy) {
            case UP:
                checkNum -= COLUMN_COUNT;
                break;
            case LEFT:
                checkNum--;
                break;
            case RIGHT:
                checkNum++;
                break;
            case DOWN:
                checkNum += COLUMN_COUNT;
                break;
        }
        if (checkNum < 0 || checkNum > MAX_POSITION_IN_SCREEN)
            return false;
    } while (!render->busyPositionScreen[checkNum]);
    return checkNum == numPositionUser;
}

bool iSee(sf::Vector2f directionSelf, sf::Vector2f posSelf, sf::Vector2f posTarget, float sector = SECTOR_VIEW)
{
    bool iSee = false;
    sf::Vector2f directionForTarget = posTarget - posSelf;
    float directionForTargetModule = getModule(directionForTarget);
    sf::Vector2f directionForUserNormal = {
            directionForTarget.x / directionForTargetModule,
            directionForTarget.y / directionForTargetModule
    };
    float scalar = directionForUserNormal.x * directionSelf.x + directionForUserNormal.y * directionSelf.y;
    if (scalar >= sector)
        iSee = true;
    return  iSee;
}

void fixPositionInRange(sf::Sprite& sprite)
{
    auto positionCurrent = sf::Vector2i(sprite.getPosition());
    int modX = (positionCurrent.x - int(SIZE_TANK / 2)) % int(SIZE_TANK);
    int modY = (positionCurrent.y - int(SIZE_TANK / 2)) % int(SIZE_TANK);

    if(modX)
    {
        if (modX > int(SIZE_TANK / 2))
        positionCurrent.x += int(SIZE_TANK) - modX;
        else
        positionCurrent.x -= modX;
        sprite.setPosition(sf::Vector2f(positionCurrent));
    }

    if(modY)
    {
        if (modY > int(SIZE_TANK / 2))
            positionCurrent.y += int(SIZE_TANK) - modY;
        else
            positionCurrent.y -= modY;
        sprite.setPosition(sf::Vector2f(positionCurrent));
    }
}

void shootUser(flecs::iter& it, size_t index, sf::Sprite& spriteUser, Moving& moving, User& user)
{
    auto render = it.world().get<Render>();
    float currentTime = render->clock.getElapsedTime().asSeconds();

    if (currentTime > user.nextTimeShoot)
    {
        user.nextTimeShoot = currentTime + 1 / SHOOT_SPEED_USER;

        Bullet bullet = {
                .isUser = true,
                .damage = USER_DAMAGE,
        };

        sf::Sprite spriteBullet;
        spriteBullet.setTexture(render->userBulletTexture);
        setOriginUpCenter(spriteBullet);
        setDirection(spriteBullet, moving.direction);
        sf::Vector2f pos = spriteUser.getPosition();
        spriteBullet.setPosition(pos);

        Moving movingBullet = {
                .direction = moving.direction,
                .speed = SPEED_BULLET,
                .nextTimeDirection = float(INT_MAX),
                .preTimeMoving = currentTime
        };

        Collisional collisionalBullet = {
                .iCantMove = false
        };

        it.world().entity()
                .set<Bullet>(bullet)
                .set<sf::Sprite>(spriteBullet)
                .set<Moving>(movingBullet)
                .set<Collisional>(collisionalBullet);
    }
}

void shootEnemy(flecs::iter& it, size_t index, sf::Sprite& spriteEnemy, Moving& moving, Enemy& enemy)
{
    auto render = it.world().get<Render>();
    float currentTime = render->clock.getElapsedTime().asSeconds();

    if (currentTime > enemy.nextTimeShoot)
    {
        enemy.nextTimeShoot = currentTime + 1 / SHOOT_SPEED_ENEMY;

        Bullet bullet = {
                .isUser = false,
                .damage = ENEMY_DAMAGE,
        };

        sf::Sprite spriteBullet;
        spriteBullet.setTexture(render->enemyBulletTexture);
        setOriginUpCenter(spriteBullet);
        setDirection(spriteBullet, moving.direction);
        sf::Vector2f pos = spriteEnemy.getPosition();
        spriteBullet.setPosition(pos);

        Moving movingBullet = {
                .direction = moving.direction,
                .speed = SPEED_BULLET,
                .nextTimeDirection = float(INT_MAX),
                .preTimeMoving = currentTime
        };

        Collisional collisionalBullet = {
                .iCantMove = false
        };

        it.world().entity()
                .set<Bullet>(bullet)
                .set<sf::Sprite>(spriteBullet)
                .set<Moving>(movingBullet)
                .set<Collisional>(collisionalBullet);
    }
}

void shootEnemyAI(flecs::iter& it, size_t index, sf::Sprite& spriteEnemyAI, Moving& moving, EnemyAI& enemyAI)
{
    auto render = it.world().get<Render>();
    float currentTime = render->clock.getElapsedTime().asSeconds();

    if (currentTime > enemyAI.nextTimeShoot)
    {
        enemyAI.nextTimeShoot = currentTime + 1 / SHOOT_SPEED_ENEMY_AI;

        Bullet bullet = {
                .isUser = false,
                .damage = ENEMY_AI_DAMAGE,
        };

        sf::Sprite spriteBullet;
        spriteBullet.setTexture(render->enemyBulletTexture);
        setOriginUpCenter(spriteBullet);
        setDirection(spriteBullet, moving.direction);
        sf::Vector2f pos = spriteEnemyAI.getPosition();
        spriteBullet.setPosition(pos);

        Moving movingBullet = {
                .direction = moving.direction,
                .speed = SPEED_BULLET,
                .nextTimeDirection = float(INT_MAX),
                .preTimeMoving = currentTime
        };

        Collisional collisionalBullet = {
                .iCantMove = false
        };

        it.world().entity()
                .set<Bullet>(bullet)
                .set<sf::Sprite>(spriteBullet)
                .set<Moving>(movingBullet)
                .set<Collisional>(collisionalBullet);
    }
}