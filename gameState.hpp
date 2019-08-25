#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <SFML/Graphics/RenderWindow.hpp>

class gameStateClass
{
public:
    virtual ~gameStateClass() {}
    virtual void update(sf::RenderWindow& window) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
};

#endif
