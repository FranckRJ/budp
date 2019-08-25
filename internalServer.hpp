#ifndef INTERNALSERVER_HPP
#define INTERNALSERVER_HPP

#include <SFML/Graphics/RenderWindow.hpp>

#include "server.hpp"
#include "gamePlayed.hpp"
#include "component.hpp"

class internalServerClass : public serverClass
{
public:
    internalServerClass();
    void update() override;
    void draw(sf::RenderWindow& window) override;
    void setInputKey(directionEnum direction) override;
private:
    gamePlayedClass gamePlayed;
    int idOfPlayer;
};

#endif
