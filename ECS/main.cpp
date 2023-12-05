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
//    window.render();
//    window.display();
//}

int main(int argc, char *argv[])
{
    flecs::world world(argc, argv);
    initWorld(world);





    while (world.progress()) {}

//    while (window.isOpen())
//    {
//        pollEvents(window);
//        update();
//        redrawFrame(window);
//                window.clear();
//                window.render();
//                window.display();
//    }
}