#pragma ONCE
#include "../render/render.h"

void initCollisional(flecs::world& world)
{
    world.filter_builder<Collisional>()
        .build()
        .each([](Collisional& collision) {
            collision.iCantMove = false;
        });

    world.system<sf::Sprite, Moving, Collisional>()
        .each([&world](
                flecs::entity e,
                sf::Sprite& sprite,
                Moving& moving,
                Collisional& collisional) {

            sf::Vector2f positionSelf = sprite.getPosition();
            directionEnum directionSelf = moving.direction;
            if (positionSelf.x < float(SIZE_TANK) / 2 && directionSelf == LEFT)
                collisional.iCantMove = true;
            else if (positionSelf.x > WINDOW_WIDTH - float(SIZE_TANK) / 2 && directionSelf == RIGHT)
                collisional.iCantMove = true;
            else if (positionSelf.y < float(SIZE_TANK) / 2 && directionSelf == UP)
                collisional.iCantMove = true;
            else if (positionSelf.y > WINDOW_HEIGHT - float(SIZE_TANK) / 2 && directionSelf == DOWN)
                collisional.iCantMove = true;

            if (!collisional.iCantMove)
                world.filter_builder<sf::Sprite>()
                    .build()
                    .each([&](const sf::Sprite& sprite) {
                        sf::Vector2f positionTarget = sprite.getPosition();
                        sf::Vector2f directionForTarget = positionTarget - positionSelf;
                        float module = getModule(directionForTarget);
                        if (module <= SIZE_TANK)
                            collisional.iCantMove = iSee(
                                    DIRECTIONS[directionSelf],
                                    positionSelf,
                                    positionTarget,
                                    0.50);
                    });


        }).add(flecs::PreUpdate);

    world.system<Collisional, Bullet, sf::Sprite, Moving>()
        .each([&](
                flecs::entity e,
                Collisional& collisional,
                Bullet& bullet,
                sf::Sprite& sprite,
                Moving& moving) {

            sf::Vector2f positionSelf = sprite.getPosition();
            directionEnum directionSelf = moving.direction;

            if (positionSelf.x < 0.f && directionSelf == LEFT)
                collisional.iOverScreen = true;
            else if (positionSelf.x > WINDOW_WIDTH && directionSelf == RIGHT)
                collisional.iOverScreen = true;
            else if (positionSelf.y < 0.f && directionSelf == UP)
                collisional.iOverScreen = true;
            else if (positionSelf.y > WINDOW_HEIGHT && directionSelf == DOWN)
                collisional.iOverScreen = true;

            if (collisional.iOverScreen)
                e.destruct();

        }).add(flecs::PreUpdate);
}