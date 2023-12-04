#include <SFML/Graphics.hpp>
#include "flecs.h"

constexpr unsigned WINDOW_WIDTH = 800;
constexpr unsigned WINDOW_HEIGHT = 600;

void init()
{
}

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

void update()
{
}

void redrawFrame(sf::RenderWindow& window)
{
    window.clear();
    window.draw();
    window.display();
}

int main(int argc, char *argv[])
{
    flecs::world world(argc, argv);
    auto e = world.entity();

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(
            sf::VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT }),
            "Arrow",
            sf::Style::Default,
            settings);



    while (window.isOpen())
    {
        pollEvents(window);
        update();
        redrawFrame(window);
    }
}