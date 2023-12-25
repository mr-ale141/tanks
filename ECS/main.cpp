//https://www.flecs.dev/explorer/

#include "init/initWorld.h"

void showGame()
{
    flecs::world world;
    initWorld(world);
    auto render = world.get<Render>();
    while (render->window->isOpen()) {
        world.progress();
    }
}

int main() {
    AnswerEnum answer;
    Menu menu;

    answer = menu.showMainMenu();

    while (answer != EXIT)
    {
        switch (answer) {
            case START:
                showGame();
                if (isWin)
                    answer = menu.showYouWin();
                else
                    answer = menu.showYouLost();
                break;
            case SET_LEVEL:
                answer = menu.showSetLevel();
                break;
            case EXIT_TO_MENU:
                answer = menu.showMainMenu();
                break;
            default:
                std::cout << "Error answer!!\n";
                break;
        }
    }
}

//    world.set<flecs::Rest>({});
//
//    world.import<flecs::monitor>();
//
//    world.app().enable_rest().enable_monitor().run();