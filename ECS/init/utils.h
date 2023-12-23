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

bool checkBarriers(const Render* render, directionEnum directionEnemy, unsigned numPositionEnemy, unsigned numPositionTarget, bool isUserTarget)
{
    for (int i = 0; i < COUNT_PROTECT_BASE; i++)
    {
        if (NUM_POSITIONS_PROTECT_BASE_ENEMY[i] == numPositionTarget)
            return false;
    }
    unsigned checkNum = numPositionEnemy;
    int countFreeBlock = 0;
    do {
        countFreeBlock++;
        if (!isUserTarget && countFreeBlock > COUNT_FREE_BLOCKS_BEFORE_FIRE)
            return false;
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
    return checkNum == numPositionTarget;
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

class Node
{
public:
    explicit Node(unsigned positionInScreen)
    {
        numPosition = positionInScreen;
        before = 0.f;
        after = 0.f;
        sum = 0.f;
        parent = nullptr;
        isInQueue = false;
        directionForThis = UP;
    }
    void updateSum()
    {
        sum = before + after;
    }
    unsigned numPosition;
    float before;
    float after;
    float sum;
    bool isInQueue;
    std::shared_ptr<Node> parent;
    directionEnum directionForThis;
};

struct CompareNodes {
    bool operator()(std::shared_ptr<Node> const& left, std::shared_ptr<Node> const& right)
    {
        return left->sum >= right->sum;
    }
};

directionEnum getDirectionForTarget(Rand& rand, Render& render, unsigned numPositionSelf, unsigned numPositionTarget, bool& isOutOfReach)
{
    std::shared_ptr<Node> openSet[MAX_POSITION_IN_SCREEN];
    for (int i = 0; i < MAX_POSITION_IN_SCREEN; i++)
        openSet[i] = nullptr;

    openSet[numPositionSelf] = std::make_shared<Node>(numPositionSelf);

    sf::Vector2f userPosition = getPositionCenter(numPositionTarget);

    std::set<unsigned> closedSet;

    std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, CompareNodes> queue;

    openSet[numPositionSelf]->isInQueue = true;
    queue.push(openSet[numPositionSelf]);

    directionEnum direction = UP;
    while (!queue.empty())
    {
        auto currentNode = queue.top();
        currentNode->isInQueue = false;
        queue.pop();

        unsigned currentNum = currentNode->numPosition;

        if (currentNode->numPosition == numPositionTarget)
        {
            direction = currentNode->directionForThis;
            while (currentNode->parent)
            {
                direction = currentNode->directionForThis;
                currentNode = currentNode->parent;
            }
            isOutOfReach = false;
            return direction;
        }

        closedSet.insert(currentNode->numPosition);

        unsigned neighbors[4] = {MAX_POSITION_IN_SCREEN, MAX_POSITION_IN_SCREEN, MAX_POSITION_IN_SCREEN, MAX_POSITION_IN_SCREEN};
        unsigned neighbor = MAX_POSITION_IN_SCREEN;

        if (currentNum > COLUMN_COUNT)
        {
            neighbor = currentNum - COLUMN_COUNT;
            if (neighbor == numPositionTarget || !render.busyPositionScreen[neighbor])
            {
                neighbors[UP] = neighbor;
            }
        }
        if (currentNum % COLUMN_COUNT != COLUMN_COUNT - 1)
        {
            neighbor = currentNum + 1;
            if (neighbor == numPositionTarget || !render.busyPositionScreen[neighbor])
                neighbors[RIGHT] = neighbor;
        }
        if (currentNum < MAX_POSITION_IN_SCREEN - COLUMN_COUNT)
        {
            neighbor = currentNum + COLUMN_COUNT;
            if (neighbor == numPositionTarget || !render.busyPositionScreen[neighbor])
                neighbors[DOWN] = neighbor;
        }
        if (currentNum % COLUMN_COUNT != 0)
        {
            neighbor = currentNum - 1;
            if (neighbor == numPositionTarget || !render.busyPositionScreen[neighbor])
                neighbors[LEFT] = neighbor;
        }

        float newBefore = currentNode->before + 1.f;
        for (int i = 0; i < 4; i++)
        {
            unsigned numNeighbors = neighbors[i];
            if (numNeighbors == MAX_POSITION_IN_SCREEN || closedSet.count(numNeighbors))
                continue;
            std::shared_ptr<Node> neighborNode = openSet[numNeighbors];
            float newAfter = 0;
            if (neighborNode)
            {
                if (neighborNode->isInQueue && newBefore < neighborNode->before)
                {
                    int queueSize = int(queue.size());
                    std::shared_ptr<Node> hold[queueSize];
                    for (int index = 0; index < queueSize; index++)
                    {
                        hold[index] = queue.top();
                        queue.pop();
                        if (hold[index]->numPosition == numNeighbors)
                        {
                            hold[index]->before = newBefore;
                            hold[index]->updateSum();
                            hold[index]->parent = openSet[currentNode->numPosition];
                            hold[index]->directionForThis = directionEnum(i);
                        }
                    }
                    for (int index = 0; index < queueSize; index++)
                        queue.push(hold[index]);
                }
            }
            else
            {
                auto newNode = std::make_shared<Node>(numNeighbors);
                openSet[numNeighbors] = newNode;
                newNode->before = newBefore;
                sf::Vector2f currentPosition = getPositionCenter(numNeighbors);
                float module = getModule(userPosition - currentPosition);
                newAfter = module / SIZE_TANK;
                newNode->after = newAfter;
                newNode->updateSum();
                newNode->parent = openSet[currentNode->numPosition];
                newNode->directionForThis = directionEnum(i);
                newNode->isInQueue = true;
                queue.push(newNode);
            }
        }
    }
    return directionEnum(rand.getRandomInt(0, 3));
}

directionEnum getDirectionEnemyAI(Rand& rand, Render& render, unsigned numPositionSelf, unsigned numPositionUser)
{
    bool isOutOfReach = true;
    directionEnum direction;
    direction = getDirectionForTarget(rand, render, numPositionSelf, NUM_POSITION_BASE_USER, isOutOfReach);
    if (isOutOfReach)
        return getDirectionForTarget(rand, render, numPositionSelf, numPositionUser, isOutOfReach);
    else
        return direction;
}

directionEnum getDirectionEnemy(Rand& rand, Render& render, unsigned numPositionSelf, unsigned numPositionUser, uint64_t id)
{
    bool isOutOfReach = true;
//    unsigned indexTarget = rand.getRandomInt(0, COUNT_PROTECT_BASE - 1);
    unsigned indexTarget = id % COUNT_PROTECT_BASE;
    unsigned  numPositionTarget = NUM_POSITIONS_PROTECT_BASE_USER[indexTarget];
    directionEnum direction;
    direction = getDirectionForTarget(rand, render, numPositionSelf, numPositionTarget, isOutOfReach);
    if (isOutOfReach || !render.busyPositionScreen[numPositionTarget])
        return getDirectionForTarget(rand, render, numPositionSelf, numPositionUser, isOutOfReach);
    else
        return direction;
}