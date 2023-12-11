#pragma ONCE
#include <SFML/System.hpp>
#include "../moving/moving.h"

void initControl(flecs::world& world)
{
    Control control;
    world.set<Control>(control);

    world.system<Render, Control>()
            .term_at(1).singleton()
            .term_at(2).singleton()
            .each([](Render& render, Control& control) {
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
            }).add(flecs::PreUpdate);

    world.system<sf::Sprite, Moving, User>()
            .each([&](flecs::iter& it, size_t index, sf::Sprite& spriteUser, Moving& moving, User& user) {
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
                    shootUser(it, index, spriteUser, moving, user);

            }).add(flecs::PreUpdate);
}