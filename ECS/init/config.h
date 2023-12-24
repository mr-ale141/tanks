#pragma ONCE
#include <SFML/Graphics.hpp>
#include <random>
#include <iostream>
#include <string>
#include <climits>
#include <set>
#include <queue>
#include <memory>
#include "flecs.h"

unsigned LEVEL = 0;

bool isWin = false;

unsigned MAX_ENEMY = 5;
unsigned MAX_ENEMY_AI = 5;

const float SECTOR_VIEW = 0.999;
const float TIME_WAITING = 0.f;

constexpr unsigned WINDOW_WIDTH = 800;
constexpr unsigned WINDOW_HEIGHT = 600;
constexpr unsigned SIZE_TANK = 40;

constexpr unsigned ROW_COUNT = int(WINDOW_HEIGHT / SIZE_TANK);
constexpr unsigned COLUMN_COUNT = int(WINDOW_WIDTH / SIZE_TANK);
constexpr unsigned MAX_POSITION_IN_SCREEN = COLUMN_COUNT * ROW_COUNT;

constexpr unsigned NUM_POSITION_USER = 249;
constexpr unsigned NUM_POSITION_BASE_USER = 289;
constexpr unsigned COUNT_PROTECT_BASE = 5;
constexpr unsigned NUM_POSITIONS_PROTECT_BASE_USER[COUNT_PROTECT_BASE] = {288, 268, 269, 270, 290};
constexpr unsigned NUM_POSITIONS_PROTECT_BASE_ENEMY[COUNT_PROTECT_BASE] = {8, 28, 29, 30, 10};

constexpr unsigned NUM_POSITION_BASE_ENEMY = 9;

float SPEED_USER = 50.f;
float SPEED_ENEMY_AI = 30.f;
float SPEED_ENEMY = 40.f;
const sf::Vector2i RANGE_RAND_DIRECTION = {1, 4};

float SPEED_BULLET = 300.f;

float SHOOT_SPEED_USER = 2.f;
float SHOOT_SPEED_ENEMY = 1.f / 2.f;
float SHOOT_SPEED_ENEMY_AI = 1.f / 3.f;
const int HP_USER = 100;
const int HP_BASE = 1;
const int HP_ENEMY = 1;
const int HP_ENEMY_AI = 2;

const int COUNT_COLUMN_TEXTURE_FIRE = 16;
const int SIZE_TEXTURE_FIRE = 64;
const float STEP_UPDATE_FIRE = 0.05;
const int COUNT_FREE_BLOCKS_BEFORE_FIRE = 3;

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

const unsigned wallWoodMaps[10][300] = {{
        300,300,300,300,300,300,300,300,8,300,10,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,28,29,30,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,106,107,108,109,110,111,112,113,300,300,300,300,300,300,
        300,300,300,300,300,300,126,127,128,129,130,131,132,133,300,300,300,300,300,300,
        140,141,142,143,144,300,146,147,148,149,150,151,152,153,300,155,156,157,158,159,
        300,300,300,300,300,300,166,167,168,169,170,171,172,173,300,300,300,300,300,300,
        300,300,300,300,300,300,186,187,188,189,190,191,192,193,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,268,269,270,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,288,300,290,300,300,300,300,300,300,300,300,300
    }, {
        300,300,300,300,300,300,300,300,8,300,10,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,28,29,30,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,65,66,300,300,300,300,300,72,73,300,300,300,300,300,300,
        300,300,300,300,300,300,86,87,300,300,300,91,92,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,107,300,300,300,111,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,127,300,300,300,131,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,148,300,150,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,268,269,270,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,288,300,290,300,300,300,300,300,300,300,300,300
    }, {
        300,300,300,300,300,300,300,300,8,300,10,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,28,29,30,300,300,300,300,300,300,300,300,300,
        300,41,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,58,300,
        300,61,300,300,300,300,300,67,300,300,300,71,300,300,300,300,300,300,78,300,
        300,81,300,300,300,300,300,87,300,300,300,91,300,300,300,300,300,300,98,300,
        300,101,300,300,300,300,300,107,300,300,300,111,300,300,300,300,300,300,118,300,
        300,121,300,300,300,300,300,127,300,300,300,131,300,300,300,300,300,300,138,300,
        300,141,142,143,144,145,146,147,300,300,300,151,152,153,154,155,156,157,158,300,
        300,161,300,300,300,300,300,167,300,300,300,171,300,300,300,300,300,300,178,300,
        300,181,300,300,300,300,300,187,300,300,300,191,300,300,300,300,300,300,198,300,
        300,201,300,300,300,300,300,207,300,300,300,211,300,300,300,300,300,300,218,300,
        300,221,300,300,300,300,300,227,300,300,300,231,300,300,300,300,300,300,238,300,
        300,241,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,258,300,
        300,300,300,300,300,300,300,300,268,269,270,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,288,300,290,300,300,300,300,300,300,300,300,300
    }, {
        300,300,300,300,300,300,300,300,8,300,10,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,28,29,30,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        140,300,142,300,144,300,146,300,148,300,150,300,152,300,154,300,156,300,158,300,
        300,161,300,163,300,165,300,167,300,169,300,171,300,173,300,175,300,177,300,179,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,268,269,270,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,288,300,290,300,300,300,300,300,300,300,300,300
    }, {
        300,300,300,300,300,300,300,300,8,300,10,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,28,29,30,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        140,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,159,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,209,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,268,269,270,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,288,300,290,300,300,300,300,300,300,300,300,300
    }, {
        300,300,300,300,300,300,300,300,8,300,10,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,28,29,30,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,61,300,63,300,65,300,67,300,69,300,71,300,73,300,75,300,77,300,79,
        300,81,300,83,300,85,300,87,300,89,300,91,300,93,300,95,300,97,300,99,
        300,101,300,103,300,105,300,107,300,109,300,111,300,113,300,115,300,117,300,119,
        300,121,300,123,300,125,126,127,128,129,130,131,132,133,300,135,300,137,300,139,
        140,141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,
        300,161,300,163,300,165,166,167,168,169,170,171,172,173,300,175,300,177,300,179,
        300,181,300,183,300,185,300,187,300,189,300,191,300,193,300,195,300,197,300,199,
        300,201,300,203,300,205,300,207,300,209,300,211,300,213,300,215,300,217,300,219,
        300,221,300,223,300,225,300,227,300,229,300,231,300,233,300,235,300,237,300,239,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,268,269,270,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,288,300,290,300,300,300,300,300,300,300,300,300
    }, {
        300,300,300,300,300,300,300,300,8,300,10,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,28,29,30,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,224,225,226,227,228,229,230,231,232,233,234,300,300,300,300,300,
        300,300,300,300,244,300,300,300,300,300,300,300,300,300,254,300,300,300,300,300,
        300,300,300,300,264,300,300,300,268,269,270,300,300,300,274,300,300,300,300,300,
        300,300,300,300,284,300,300,300,288,300,290,300,300,300,294,300,300,300,300,300
    }, {
        300,300,300,300,300,300,300,300,8,300,10,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,28,29,30,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,
        140,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,159,
        160,300,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,300,300,179,
        180,300,182,300,300,300,300,300,300,300,300,300,300,300,300,300,196,300,300,199,
        200,300,202,300,300,300,300,300,300,300,300,300,300,300,300,300,216,300,300,219,
        220,300,222,300,300,225,226,227,228,229,230,231,232,233,300,300,236,300,300,239,
        240,300,242,300,300,245,300,300,300,300,300,300,300,253,300,300,256,300,300,259,
        260,300,262,300,300,265,300,300,268,269,270,300,300,273,300,300,276,300,300,279,
        280,300,282,300,300,285,300,300,288,300,290,300,300,293,300,300,296,300,300,299
    }, {
        300,300,300,300,300,300,300,300,8,300,10,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,28,29,30,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,165,166,167,168,169,170,171,172,173,300,300,300,300,300,300,
        300,300,300,300,300,185,300,300,300,300,300,300,300,193,300,300,300,300,300,300,
        300,300,300,300,300,205,300,300,300,300,300,300,300,213,300,300,300,300,300,300,
        300,300,300,300,300,225,300,300,300,300,300,300,300,233,300,300,300,300,300,300,
        300,300,300,300,300,245,300,300,300,300,300,300,300,253,300,300,300,300,300,300,
        300,300,300,300,300,265,300,300,268,269,270,300,300,273,300,300,300,300,300,300,
        300,300,300,300,300,285,300,300,288,300,290,300,300,293,300,300,300,300,300,300
    }, {
        300,300,300,300,300,300,300,300,8,300,10,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,28,29,30,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,
        120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,268,269,270,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,288,300,290,300,300,300,300,300,300,300,300,300
}};

const unsigned wallMetalMaps[10][300] = {{
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,85,86,87,88,89,90,91,92,93,94,300,300,300,300,300,
        300,300,300,300,300,105,300,300,300,300,300,300,300,300,114,300,300,300,300,300,
        300,300,300,300,300,125,300,300,300,300,300,300,300,300,134,300,300,300,300,300,
        300,300,300,300,300,145,300,300,300,300,300,300,300,300,154,300,300,300,300,300,
        300,300,300,300,300,165,300,300,300,300,300,300,300,300,174,300,300,300,300,300,
        300,300,300,300,300,185,300,300,300,300,300,300,300,300,194,300,300,300,300,300,
        300,300,300,300,300,205,206,207,208,209,210,211,212,213,214,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300
   }, {
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,61,62,63,300,300,300,300,300,300,300,300,300,300,300,75,76,77,78,300,
        300,300,300,83,300,300,300,300,300,300,300,300,300,300,300,95,300,300,300,300,
        300,300,300,103,300,300,300,300,300,300,300,300,300,300,300,115,300,300,300,300,
        300,300,300,300,124,300,300,300,300,300,300,300,300,300,134,300,300,300,300,300,
        300,300,300,300,300,145,300,300,300,149,300,300,300,153,300,300,300,300,300,300,
        300,300,300,300,300,300,166,300,300,300,300,300,172,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,187,188,300,190,191,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,208,300,210,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300
    }, {
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,69,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,89,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,109,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,129,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,149,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,169,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,189,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,209,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,229,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300
    }, {
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        120,300,122,300,124,300,126,300,128,300,130,300,132,300,134,300,136,300,138,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,181,300,183,300,185,300,187,300,189,300,191,300,193,300,195,300,197,300,199,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300
    }, {
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,68,300,70,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,87,300,300,300,91,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,106,300,300,109,300,300,112,300,300,300,300,300,300,300,
        300,300,300,300,300,125,300,300,128,300,130,300,300,133,300,300,300,300,300,300,
        300,141,142,143,144,145,300,147,300,300,300,151,300,153,154,155,156,157,158,300,
        300,300,300,300,300,165,300,300,168,300,170,300,300,173,300,300,300,300,300,300,
        300,300,300,300,300,300,186,300,300,189,300,300,192,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,207,300,300,300,211,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,228,300,230,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300
    }, {
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,241,300,243,300,245,300,300,300,300,300,300,300,253,300,255,300,257,300,259,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300
    }, {
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,61,62,63,300,300,66,67,68,300,300,71,72,73,300,300,76,77,78,300,
        300,81,82,83,300,300,86,87,88,300,300,91,92,93,300,300,96,97,98,300,
        300,101,102,103,300,300,106,107,108,300,300,111,112,113,300,300,116,117,118,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        140,141,300,143,144,145,146,300,148,149,150,151,300,153,154,155,156,300,158,159,
        160,161,300,163,164,165,166,300,168,169,170,171,300,173,174,175,176,300,178,179,
        180,181,300,183,184,185,186,300,188,189,190,191,300,193,194,195,196,300,198,199,
        200,201,300,203,204,205,206,300,208,209,210,211,300,213,214,215,216,300,218,219,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300
    }, {
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,22,23,24,300,300,300,300,300,300,300,300,300,300,35,36,37,300,300,
        300,300,42,43,44,300,300,300,300,300,300,300,300,300,300,55,56,57,300,300,
        300,300,62,63,64,300,300,300,300,69,300,300,300,300,300,75,76,77,300,300,
        300,300,82,83,84,300,300,300,300,89,300,300,300,300,300,95,96,97,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300
    }, {
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        80,81,300,83,84,300,86,87,300,89,90,300,92,93,300,95,96,300,98,99,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,142,300,300,300,300,300,300,300,300,300,300,300,300,300,156,300,300,300,
        300,300,162,300,300,300,300,300,300,300,300,300,300,300,300,300,176,300,300,300,
        300,300,182,300,300,300,300,300,300,300,300,300,300,300,300,300,196,300,300,300,
        300,300,202,300,300,300,300,300,300,300,300,300,300,300,300,300,216,300,300,300,
        300,300,222,300,300,300,300,300,300,300,300,300,300,300,300,300,236,300,300,300,
        300,300,242,300,300,300,300,300,300,300,300,300,300,300,300,300,256,300,300,300,
        300,300,262,300,300,300,300,300,300,300,300,300,300,300,300,300,276,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300
    }, {
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,162,163,164,300,300,300,300,300,300,300,300,300,174,175,176,300,300,300,
        300,300,182,183,184,300,300,300,300,300,300,300,300,300,194,195,196,300,300,300,
        300,300,202,203,204,300,300,300,300,300,300,300,300,300,214,215,216,300,300,300,
        300,300,222,223,224,300,300,300,300,300,300,300,300,300,234,235,236,300,300,300,
        300,300,242,243,244,300,300,300,300,300,300,300,300,300,254,255,256,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
        300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300
}};

