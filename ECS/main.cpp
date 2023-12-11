//https://www.flecs.dev/explorer/

#include "init/initWorld.h"

int main(int argc, char *argv[]) {
    flecs::world world(argc, argv);

    initWorld(world);

//    world.set<flecs::Rest>({});
//
//    world.import<flecs::monitor>();
//
//    world.app().enable_rest().enable_monitor().run();

    auto render = world.get<Render>();

    while (render->window->isOpen()) {
        world.progress();
    }
}