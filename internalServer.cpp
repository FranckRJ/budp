#include "internalServer.hpp"

internalServerClass::internalServerClass()
{
    gamePlayed.loadLevelFromFile("level1");
    gamePlayed.addPlayer();
    idOfPlayer = gamePlayed.addPlayer();
}

void internalServerClass::update()
{
    gamePlayed.update();
}

void internalServerClass::draw(sf::RenderWindow& window)
{
    gamePlayed.draw(window);
}

void internalServerClass::setInputKey(directionEnum direction)
{
    gamePlayed.setInputOfThisEntity(idOfPlayer, direction);
}
