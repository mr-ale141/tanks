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
        before = 0;
        after = 0;
        sum = 0;
        parent = nullptr;
        isInQueue = false;
        directionForThis = UP;
    }
    bool operator<(const Node& right) const
    {
        return sum > right.sum;
    }
    bool operator>(const Node& right) const
    {
        return sum < right.sum;
    }
    bool operator==(const Node& right) const
    {
        return sum == right.sum;
    }
    bool operator>=(const Node& right) const
    {
        return sum <= right.sum;
    }
    bool operator<=(const Node& right) const
    {
        return sum >= right.sum;
    }
    bool operator!=(const Node& right) const
    {
        return sum != right.sum;
    }
    void updateSum()
    {
        sum = before + after;
    }
    unsigned numPosition;
    int before;
    int after;
    int sum;
    bool isInQueue;
    Node* parent;
    directionEnum directionForThis;
};

directionEnum getDirectionAI(Rand& rand, Render& render, unsigned numPositionSelf, unsigned numPositionUser)
{
    auto startNode = Node(numPositionSelf);
    auto endNode = Node(numPositionUser);
    sf::Vector2f userPosition = getPositionCenter(numPositionUser);
    auto zeroNode = Node(MAX_POSITION_IN_SCREEN);
    std::set<unsigned> closedSet;
    Node* openSet[MAX_POSITION_IN_SCREEN];
    for (int i = 0; i < MAX_POSITION_IN_SCREEN; i++)
        openSet[i] = nullptr;
    openSet[startNode.numPosition] = &startNode;

    std::priority_queue<Node> queue;
    queue.push(startNode);
    startNode.isInQueue = true;

    directionEnum direction = UP;
    while (!queue.empty())
    {
        auto node = &queue.top();
        unsigned currentNum = node->numPosition;
        auto currentNode = openSet[currentNum];

        if (currentNode->numPosition == endNode.numPosition)
        {
            while (currentNode)
            {
                direction = currentNode->directionForThis;
//                int beforeNum = int(currentNode->numPosition);
                currentNode = currentNode->parent;
//                int currentNum = int(currentNode->numPosition);
//                int delta = currentNum - beforeNum;
//                switch (delta) {
//                    case int(COLUMN_COUNT):
//                        direction = UP;
//                        break;
//                    case -1:
//                        direction = RIGHT;
//                        break;
//                    case -int(COLUMN_COUNT):
//                        direction = DOWN;
//                        break;
//                    case 1:
//                        direction = LEFT;
//                        break;
//                    default:
//                        std::cout << "Error find path!!!\n";
//                        break;
//                }

            }
            return direction;
        }

        closedSet.insert(currentNode->numPosition);

        unsigned neighbors[4] = {MAX_POSITION_IN_SCREEN, MAX_POSITION_IN_SCREEN, MAX_POSITION_IN_SCREEN, MAX_POSITION_IN_SCREEN};
        unsigned neighbor = MAX_POSITION_IN_SCREEN;

        if (currentNum > COLUMN_COUNT)
        {
            neighbor = currentNum - COLUMN_COUNT;
            if (!render.busyPositionScreen[neighbor])
            {
                neighbors[UP] = neighbor;
            }
        }
        if (currentNum % COLUMN_COUNT != COLUMN_COUNT - 1)
        {
            neighbor = currentNum + 1;
            if (!render.busyPositionScreen[neighbor])
                neighbors[RIGHT] = neighbor;
        }
        if (currentNum < MAX_POSITION_IN_SCREEN - COLUMN_COUNT)
        {
            neighbor = currentNum + COLUMN_COUNT;
            if (!render.busyPositionScreen[neighbor])
                neighbors[DOWN] = neighbor;
        }
        if (currentNum % COLUMN_COUNT != 0)
        {
            neighbor = currentNum - 1;
            if (!render.busyPositionScreen[neighbor])
                neighbors[LEFT] = neighbor;
        }

        int newBefore = currentNode->before + 1;
        for (int i = 0; i < 4; i++)
        {
            currentNum = neighbors[i];
            if (currentNum == MAX_POSITION_IN_SCREEN || closedSet.count(currentNum))
                continue;
            Node* neighborNode = openSet[currentNum];
            int newAfter = 0;
            if (neighborNode)
            {
                if (neighborNode->isInQueue && newBefore < neighborNode->before)
                {
                    neighborNode->before = newBefore;
                    sf::Vector2f currentPosition = getPositionCenter(currentNum);
                    float module = getModule(userPosition - currentPosition);
                    newAfter = int(module / SIZE_TANK);
                    neighborNode->after = newAfter;
                    neighborNode->updateSum();
                    neighborNode->parent = openSet[currentNode->numPosition];
                    neighborNode->directionForThis = directionEnum(i);
                    queue.push(zeroNode);
                    std::cout << queue.top().numPosition << std::endl;
                    queue.pop();
                    std::cout << queue.top().numPosition << std::endl;
                }
            }
            else
            {
                auto newNode = Node(currentNum);
                openSet[currentNum] = &newNode;
                newNode.before = newBefore;
                sf::Vector2f currentPosition = getPositionCenter(currentNum);
                float module = getModule(userPosition - currentPosition);
                newAfter = int(module / SIZE_TANK);
                newNode.after = newAfter;
                newNode.updateSum();
                newNode.parent = openSet[currentNode->numPosition];
                newNode.directionForThis = directionEnum(i);
                newNode.isInQueue = true;
                queue.push(newNode);
            }
        }
        currentNode->isInQueue = false;
        queue.pop();
    }

    return directionEnum(rand.getRandomInt(0, 3));
}