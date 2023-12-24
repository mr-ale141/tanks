//https://www.flecs.dev/explorer/

#include "init/initWorld.h"

void showGame(flecs::world& world)
{
    initWorld(world);
    auto render = world.get<Render>();
    while (render->window->isOpen()) {
        world.progress();
    }
}

int main(int argc, char *argv[]) {

    flecs::world world(argc, argv);

    showGame(world);
}

//    world.set<flecs::Rest>({});
//
//    world.import<flecs::monitor>();
//
//    world.app().enable_rest().enable_monitor().run();