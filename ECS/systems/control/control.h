#pragma ONCE
#include <SFML/System.hpp>
#include "../moving/moving.h"

void initControl(flecs::world& world)
{

    world.system<Render>()
            .term_at(1).singleton()
            .each([](Render& render) {
                sf::Event event = {};
                while(render.window->pollEvent(event))
                {
                    switch (event.type)
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
                directionEnum oldDirection = moving.direction;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
                    shootUser(it, index, spriteUser, moving, user);
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
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

                if (oldDirection != moving.direction)
                {
                    setDirection(spriteUser, moving.direction);
                    fixPositionInRange(spriteUser);
                }

            }).add(flecs::PreUpdate);
}