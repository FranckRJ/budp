#ifndef GAMESTATESTACK_HPP
#define GAMESTATESTACK_HPP

#include <SFML/Graphics/RenderWindow.hpp>

#include <list>
#include <memory> //std::unique_ptr

#include "gameState.hpp"

class gameStateStackClass
{
public:
    gameStateStackClass();
    void set(gameStateClass* state);
    void add(gameStateClass* state);
    void addBefore(gameStateClass* state);
    void pop();
    void popBefore();
    void update(sf::RenderWindow& window);
    void oldUpdate(sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);
    void oldDraw(sf::RenderWindow& window);
    bool getChange() const;
    void setChange(bool newChange);
private:
    std::list<std::unique_ptr<gameStateClass>> listOfState;
    bool change;
};

#endif
