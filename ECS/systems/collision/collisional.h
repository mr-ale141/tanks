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
                    Collisional& collision) {

                sf::Vector2f positionSelf = sprite.getPosition();
                directionEnum directionSelf = moving.direction;

                if (positionSelf.x < float(SIZE_TANK) / 2 && directionSelf == LEFT)
                    collision.iCantMove = true;
                else if (positionSelf.x > WINDOW_WIDTH - float(SIZE_TANK) / 2 && directionSelf == RIGHT)
                    collision.iCantMove = true;
                else if (positionSelf.y < float(SIZE_TANK) / 2 && directionSelf == UP)
                    collision.iCantMove = true;
                else if (positionSelf.y > WINDOW_HEIGHT - float(SIZE_TANK) / 2 && directionSelf == DOWN)
                    collision.iCantMove = true;

//                if (!collision.iCantMove)
//                    world.filter_builder<sf::Sprite, WallWood, WallMetal>()
//                            .term_at(2).or_()
//                            .build()
//                            .each([&](
//                                    const sf::Sprite& spriteWall,
//                                    const WallWood&,
//                                    const WallMetal&) {
//                                sf::Vector2f positionWall = spriteWall.getPosition();
//                                sf::Vector2f directionForTarget = positionWall - positionSelf;
//                                float module = getModule(directionForTarget);
//                                if (module <= SIZE_TANK)
//                                    collision.iCantMove = iSee(
//                                            DIRECTIONS[directionSelf],
//                                            positionSelf,
//                                            positionWall,
//                                            0.50);
//                            });

                if (!collision.iCantMove)
                    world.filter_builder<sf::Sprite>()
                            .build()
                            .each([&](const sf::Sprite& sprite) {
                                sf::Vector2f positionTarget = sprite.getPosition();
                                sf::Vector2f directionForTarget = positionTarget - positionSelf;
                                float module = getModule(directionForTarget);
                                if (module <= SIZE_TANK)
                                    collision.iCantMove = iSee(
                                            DIRECTIONS[directionSelf],
                                            positionSelf,
                                            positionTarget,
                                            0.50);
                            });

            }).add(flecs::PreUpdate);
}