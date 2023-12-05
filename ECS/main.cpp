#include "utils/initWorld.h"

//void init()
//{
//}

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

//void update()
//{
//}

//void redrawFrame(sf::RenderWindow& window)
//{
//    window.clear();
//    window.draw();
//    window.display();
//}

int main(int argc, char *argv[])
{
    flecs::world world(argc, argv);
    initWorld(world);

    auto render = world.get<Render>();

    while(1)
    {
        render->window->clear();

        world.filter_builder<sf::Sprite, Render>()
                .term_at(2).singleton()
                .build()
                .each([](sf::Sprite& sprite, Render& render) {
                    render.window->draw(sprite);
                });

        render->window->display();
    }

//    while (window.isOpen())
//    {
//        pollEvents(window);
//        update();
//        redrawFrame(window);
//                window.clear();
//                window.draw();
//                window.display();
//    }
}