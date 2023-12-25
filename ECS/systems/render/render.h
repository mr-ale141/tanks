#pragma ONCE
#include "../../menu/menu.h"

void initRenderSystems(flecs::world& world)
{
    world.system<Render>()
            .term_at(1).singleton()
            .each([](Render& render) {
                render.window->clear();
            }).add(flecs::PostUpdate);

    world.system<sf::Sprite, Render>()
            .term<Fire>().oper(flecs::Not)
            .term_at(2).singleton()
            .each([](sf::Sprite& sprite, Render& render) {
                render.window->draw(sprite);
            }).add(flecs::PreStore);

    world.system<sf::Sprite, Fire, Render>()
            .term_at(3).singleton()
            .each([](sf::Sprite& sprite, Fire& fire, Render& render) {
                render.window->draw(sprite);
            }).add(flecs::PreStore);

    world.system<Render>()
            .term_at(1).singleton()
            .each([](Render& render) {
                render.window->display();
            }).add(flecs::OnStore);
}