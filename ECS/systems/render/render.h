#pragma ONCE
#include "../../init/utils.h"

void initRenderSystems(flecs::world& world)
{
    world.system<Render>()
            .term_at(1).singleton()
            .each([](Render& render) {
                render.window->clear();
            }).add(flecs::PostUpdate);

    world.system<sf::Sprite, Render>()
            .term_at(2).singleton()
            .each([](sf::Sprite& sprite, Render& render) {
                render.window->draw(sprite);
            }).add(flecs::PreStore);

    world.system<Render>()
            .term_at(1).singleton()
            .each([](Render& render) {
                render.window->display();
            }).add(flecs::OnStore);
}