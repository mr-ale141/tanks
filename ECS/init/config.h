#pragma ONCE
#include <SFML/Graphics.hpp>
#include <random>
#include <iostream>
#include <string>
#include <climits>
#include "flecs.h"

constexpr unsigned MAX_ENEMY = 10;
constexpr unsigned MAX_ENEMY_AI = 5;
constexpr unsigned MAX_WALL_WOOD = 80;
constexpr unsigned MAX_WALL_METAL = 40;

const float SECTOR_VIEW = 0.999;
const float TIME_WAITING = 0.f;

constexpr unsigned WINDOW_WIDTH = 800;
constexpr unsigned WINDOW_HEIGHT = 600;
constexpr unsigned SIZE_TANK = 40;

constexpr unsigned ROW_COUNT = int(WINDOW_HEIGHT / SIZE_TANK);
constexpr unsigned COLUMN_COUNT = int(WINDOW_WIDTH / SIZE_TANK);
constexpr unsigned MAX_POSITION_IN_SCREEN = COLUMN_COUNT * ROW_COUNT;
constexpr unsigned NUM_POSITION_USER = (ROW_COUNT - 1) * COLUMN_COUNT + COLUMN_COUNT / 2 - 1;

const float SPEED_USER = 80.f;
const float SPEED_ENEMY_AI = 20.f;
const float SPEED_ENEMY = 40.f;
const sf::Vector2i RANGE_RAND_DIRECTION = {1, 4};

constexpr float SPEED_BULLET = 300.f;

const float SHOOT_SPEED_USER = 2.f;
const float SHOOT_SPEED_ENEMY = 1.f / 3.f;
const float SHOOT_SPEED_ENEMY_AI = 1.f / 4.f;
const int HP_USER = 3;
const int HP_ENEMY = 1;
const int HP_ENEMY_AI = 2;

const int COUNT_COLUMN_TEXTURE_FIRE = 16;
const int SIZE_TEXTURE_FIRE = 64;
const float STEP_UPDATE_FIRE = 0.05;

constexpr int USER_DAMAGE = 1;
constexpr int ENEMY_DAMAGE = 1;
constexpr int ENEMY_AI_DAMAGE = 1;

const sf::Vector2f DIRECTIONS[4] = {
        { 0.f, -1.f },
        { 1.f, 0.f },
        { 0.f, 1.f },
        { -1.f, 0.f }
};

enum directionEnum
{
    UP,
    RIGHT,
    DOWN,
    LEFT
};
