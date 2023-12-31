// cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug -S ./workshop1.1/ -B ./workshop1.1/
#include "libs/headers/world.hpp"

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;

void pollEvents(sf::RenderWindow& window, World& world)
{
    world.updateEvent();
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

void update(World& world)
{
    world.update();
}

void redrawFrame(sf::RenderWindow& window, World& world)
{
    window.clear();
    window.draw(world);
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

    sf::Clock clockStart;

    World world(WINDOW_WIDTH, WINDOW_HEIGHT, clockStart);

    while (window.isOpen())
    {
        pollEvents(window, world);
        update(world);
        redrawFrame(window, world);
    }
}