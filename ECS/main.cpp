#include "utils/initWorld.h"

int main(int argc, char *argv[]) {
    flecs::world world(argc, argv);
    initWorld(world);

    auto render = world.get<Render>();

    while (render->window->isOpen()) {
        world.progress();
    }
}