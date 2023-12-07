#pragma ONCE
#include <SFML/Graphics.hpp>
#include <random>
#include <iostream>
#include <string>
#include "flecs.h"

constexpr unsigned MAX_ENEMY = 10;
constexpr unsigned MAX_ENEMY_AI = 5;
constexpr unsigned MAX_WALL_WOOD = 100;
constexpr unsigned MAX_WALL_METAL = 50;

const float SECTOR_VIEW = 0.98;
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