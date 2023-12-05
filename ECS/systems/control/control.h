#pragma ONCE
#include <SFML/System.hpp>
#include "../moving/moving.h"

struct Control {
    sf::Event event;
};

void initControlObserver(flecs::world& world)
{
    Control control;
    world.set<Control>(control);

    world.system<Moving, User, Render, Control>()
            .term_at(3).singleton()
            .term_at(4).singleton()
            .each([](Moving& moving, User, Render& render, Control& control) {
                while(render.window->pollEvent(control.event))
                {
                    switch (control.event.type)
                    {
                        case sf::Event::Closed:
                            render.window->close();
                            break;
                        default:
                            break;
                    }
                }
                
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                {
                    moving.speed = SPEED_USER;
                    moving.direction = UP;
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                {
                    moving.speed = SPEED_USER;
                    moving.direction = LEFT;
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                {
                    moving.speed = SPEED_USER;
                    moving.direction = RIGHT;
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                {
                    moving.speed = SPEED_USER;
                    moving.direction = DOWN;
                }
                else
                    moving.speed = 0;

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
                    ;

            }).add(flecs::PreUpdate);
}

//void pollEvents(sf::RenderWindow& window)
//{
//    sf::Event event;
//    while (window.pollEvent(event))
//    {
//        switch (event.type)
//        {
//            case sf::Event::Closed:
//                window.close();
//                break;
//            default:
//                break;
//        }
//    }
//}


//    while (window.isOpen())
//    {
//        pollEvents(window);
//        update();
//        redrawFrame(window);
//                window.clear();
//                window.render();
//                window.display();
//    }