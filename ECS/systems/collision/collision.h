#pragma ONCE
#include "../render/render.h"

void initCollision(flecs::world& world)
{
    world.filter_builder<Collision>()
            .build()
            .each([](Collision& collision) {
                collision.iCantMove = false;
                collision.isHitMe = false;
            });

    world.system<sf::Sprite, Moving, Collision>()
            .each([&world](
                    flecs::entity e,
                    sf::Sprite& sprite,
                    Moving& moving,
                    Collision& collision) {

//                auto id = flecs::id(e.id());

                sf::Vector2f positionSelf = sprite.getPosition();
                directionEnum directionSelf = moving.direction;

                if (positionSelf.x < float(SIZE_TANK) / 2 && directionSelf == LEFT)
                    collision.iCantMove = true;
                if (positionSelf.x > WINDOW_WIDTH - float(SIZE_TANK) / 2 && directionSelf == RIGHT)
                    collision.iCantMove = true;
                if (positionSelf.y < float(SIZE_TANK) / 2 && directionSelf == UP)
                    collision.iCantMove = true;
                if (positionSelf.y > WINDOW_HEIGHT - float(SIZE_TANK) / 2 && directionSelf == DOWN)
                    collision.iCantMove = true;

                if (!collision.iCantMove)
                    world.filter_builder<sf::Sprite, WallWood, WallMetal>()
                            .term_at(2).or_()
                            .build()
                            .each([&](
                                    const sf::Sprite& spriteWall,
                                    const WallWood&,
                                    const WallMetal&) {
                                sf::Vector2f positionWall = spriteWall.getPosition();
                                sf::Vector2f directionForTarget = positionWall - positionSelf;
                                float module = getModule(directionForTarget);
                                if (module <= SIZE_TANK)
                                    collision.iCantMove = iSee(
                                            DIRECTIONS[directionSelf],
                                            positionSelf,
                                            positionWall,
                                            0.50);
                            });

//                if (!collision.iCantMove)
//                    world.filter_builder<sf::Sprite, WallMetal>()
//                            .build()
//                            .each([&](
//                                    const sf::Sprite& spriteWall,
//                                    const WallMetal&) {
//                                sf::Vector2f positionWall = spriteWall.getPosition();
//                                sf::Vector2f directionForTarget = positionWall - positionSelf;
//                                float module = getModule(directionForTarget);
//                                if (module <= SIZE_TANK)
//                                    collision.iCantMove = iSee(
//                                            DIRECTIONS[directionSelf],
//                                            positionSelf,
//                                            positionWall);
//                            });

            }).add(flecs::PreUpdate);
}