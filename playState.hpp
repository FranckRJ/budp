#ifndef PLAYSTATE_HPP
#define PLAYSTATE_HPP

#include <memory> //std::unique_ptr
#include <list>
#include <SFML/Graphics/RenderWindow.hpp>

#include "gameState.hpp"
#include "server.hpp"
#include "component.hpp"

class playStateClass : public gameStateClass
{
public:
    playStateClass();
    void update(sf::RenderWindow& window) override;
    void draw(sf::RenderWindow& window) override;
private:
    std::unique_ptr<serverClass> server;
    std::list<directionEnum> listOfDirection;
};

#endif
