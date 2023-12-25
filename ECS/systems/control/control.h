#pragma ONCE
#include <SFML/System.hpp>
#include "../moving/moving.h"

void initControlSystems(flecs::world& world)
{

    auto controlCloseWindow = world.system<Render>()
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
            });

    auto controlUser = world.system<sf::Sprite, Moving, User>()
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
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                {
                    isWin = false;
                    world.get_mut<Render>()->window->close();
                }
                else
                    moving.speed = 0;

                if (oldDirection != moving.direction)
                {
                    setDirection(spriteUser, moving.direction);
                    fixPositionInRange(spriteUser);
                }

            });

    controlCloseWindow.add(flecs::OnLoad);
    controlUser.add(flecs::OnLoad);
}