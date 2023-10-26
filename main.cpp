// cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug -S ./workshop1.1/ -B ./workshop1.1/
#include "ball.hpp"
#include <SFML/Graphics.hpp>

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;
constexpr int RADIUS = 50;
constexpr int CH_SIZE = 20;
const char* name = "MDM";
constexpr float G = 98.f;
constexpr float SPEED_Y = 200.f;

void pollEvents(sf::RenderWindow& window)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::Closed:
            window.close();
            break;
        default:
            break;
        }
    }
}

void update(Ball& ball, sf::Clock& clock, float& preTime)
{
    sf::Vector2f pos = ball.getPos();
    sf::Vector2f v = ball.getV();
    if (pos.y + RADIUS >= WINDOW_HEIGHT)
        v.y = -SPEED_Y;
    float time = clock.getElapsedTime().asSeconds();
    float dt = time - preTime;
    float offsetV = dt * G;
    v.y += offsetV;
    sf::Vector2f offsetY = v * dt;
    pos += offsetY;
    ball.setV(v);
    ball.setPos(pos);
    preTime = time;
}

void redrawFrame(sf::RenderWindow& window, Ball& ball)
{
    window.clear();
    window.draw(ball);
    window.display();
}

int main()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(
        sf::VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT }),
        "Circle with name",
        sf::Style::Default,
        settings);

    sf::Vector2f pos = { WINDOW_WIDTH / 2, WINDOW_HEIGHT - RADIUS };
    sf::Clock clock;
    Ball ball(RADIUS, pos, sf::Color(255, 150, 0), sf::Color(0, 100, 255), name, CH_SIZE, sf::Color(0, 0, 0));
    float preTime = clock.getElapsedTime().asSeconds();

    while (window.isOpen())
    {
        pollEvents(window);
        update(ball, clock, preTime);
        redrawFrame(window, ball);
    }
}