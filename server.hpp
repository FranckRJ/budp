#ifndef SERVER_HPP
#define SERVER_HPP

#include <SFML/Graphics/RenderWindow.hpp>

#include "component.hpp"

class serverClass
{
public:
    virtual ~serverClass() = default;
    virtual void update() = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual void setInputKey(directionEnum direction) = 0;
};

#endif
