#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <string>
#include <vector>

enum class directionEnum : int {left = 0, right = 1, up = 2, down = 3, nowhere = 4};

struct animationStruct
{
    std::vector<sf::Vector2i> listOfSprites;
    int spriteDuration;
    int spriteDurationLeft = 0;
    int currentSpriteNumber = 0;
};

struct componentBase {};

struct componentRender : public componentBase
{
    sf::Sprite sprite;
    sf::Vector2i marginOfSprite;
    sf::Vector2i spriteSize;
    bool forceYRenderPos = false;
    int yRenderPos;
};

struct componentCollideBox : public componentBase
{
    sf::Vector2i collideBoxSize;
    bool isSolid = true;
    componentCollideBox() = default;
    explicit componentCollideBox(sf::Vector2i newCollideBoxSize, bool newIsSolid = true) :
                                collideBoxSize(newCollideBoxSize),
                                isSolid(newIsSolid) {}
};

struct componentPosition : public componentBase
{
    sf::Vector2i position;
    componentPosition() = default;
    explicit componentPosition(sf::Vector2i newPosition) : position(newPosition) {}
};

struct componentInput : public componentBase
{
    bool goLeft = false;
    bool goRight = false;
    bool goUp = false;
    bool goDown = false;
};

struct componentMovable : public componentBase
{
    int speed;
    sf::Vector2i velocity = sf::Vector2i(0, 0);
    componentMovable() = default;
    explicit componentMovable(int newSpeed) : speed(newSpeed) {}
};

struct componentDirection : public componentBase
{
    directionEnum direction = directionEnum::left;
    bool isWalking = false;
    componentDirection() = default;
    explicit componentDirection(directionEnum newDirection) : direction(newDirection) {}
};

struct componentAnimation : public componentBase
{
    animationStruct animations[8];
    /*
    -0 standLeftAnimation;
    -1 standRightAnimation;
    -2 standUpAnimation;
    -3 standDownAnimation;
    -4 walkLeftAnimation;
    -5 walkRightAnimation;
    -6 walkUpAnimation;
    -7 walkDownAnimation;
    */
};

namespace componentTool
{
    componentRender* makeRenderComponent(std::string fileName, sf::Vector2i newMarginOfSprite = sf::Vector2i(0, 0),
                            sf::IntRect subTexture = sf::IntRect(-1, -1, -1, -1),
                            bool newForceYRenderPos = false, int newYRenderPos = 0);
}

#endif
