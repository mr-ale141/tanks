#pragma ONCE
#include "../moving/moving.h"

struct Control {
    sf::Event event;
    bool isWaiting;
};

void initControlObserver(flecs::world& world)
{
    Control control;
    control.isWaiting = false;
    world.set<Control>(control);

    world.system<Render, Control>()
            .term_at(1).singleton()
            .term_at(2).singleton()
            .each([](Render& render, Control& control) {
                while(render.window->pollEvent(control.event))
                {
                    switch (control.event.type)
                    {
                        case sf::Event::Closed:
                            render.window->close();
                            break;
                        default:
                            control.isWaiting = true;
                            break;
                    }
                }
            }).add(flecs::PreUpdate);
}

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


//    while (window.isOpen())
//    {
//        pollEvents(window);
//        update();
//        redrawFrame(window);
//                window.clear();
//                window.render();
//                window.display();
//    }