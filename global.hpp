#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#define WIDTH_SCREEN 960
#define HEIGHT_SCREEN 540

#include "gameStateStack.hpp"

struct global
{
    static gameStateStackClass* activeGameStateStack;
};

#endif
