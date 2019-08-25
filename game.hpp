#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics/RenderWindow.hpp>

#include "gameStateStack.hpp"

class gameClass
{
public:
    gameClass();
    void run();
private:
    gameStateStackClass gameStateStack;
    sf::RenderWindow window;
};

#endif
