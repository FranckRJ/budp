#include "gameStateStack.hpp"

gameStateStackClass::gameStateStackClass()
{
    change = false;
}

void gameStateStackClass::set(gameStateClass* state)
{
    listOfState.clear();
    add(state);
}

void gameStateStackClass::add(gameStateClass* state)
{
    change = true;
    listOfState.push_back(std::unique_ptr<gameStateClass>(state));
}

void gameStateStackClass::addBefore(gameStateClass* state)
{
    if(listOfState.size() > 0)
    {
        std::list<std::unique_ptr<gameStateClass>>::iterator it = listOfState.end();
        change = true;
        --it;
        listOfState.insert(it, std::unique_ptr<gameStateClass>(state));
    }
}

void gameStateStackClass::pop()
{
    change = true;
    listOfState.pop_back();
}

void gameStateStackClass::popBefore()
{
    if(listOfState.size() >= 2)
    {
        std::list<std::unique_ptr<gameStateClass>>::iterator it = listOfState.end();
        change = true;
        ----it;
        listOfState.erase(it);
    }
}

void gameStateStackClass::update(sf::RenderWindow& window)
{
    if(listOfState.empty() == false)
    {
        listOfState.back()->update(window);
    }
}

void gameStateStackClass::oldUpdate(sf::RenderWindow& window)
{
    if(listOfState.size() >= 2)
    {
        std::list<std::unique_ptr<gameStateClass>>::reverse_iterator rit = listOfState.rbegin();
        ++rit;
        (*rit)->update(window);
    }
}

void gameStateStackClass::draw(sf::RenderWindow& window)
{
    if(listOfState.empty() == false)
    {
        listOfState.back()->draw(window);
    }
}

void gameStateStackClass::oldDraw(sf::RenderWindow& window)
{
    if(listOfState.size() >= 2)
    {
        std::list<std::unique_ptr<gameStateClass>>::reverse_iterator rit = listOfState.rbegin();
        ++rit;
        (*rit)->draw(window);
    }
}

bool gameStateStackClass::getChange() const
{
    return change;
}

void gameStateStackClass::setChange(bool newChange)
{
    change = newChange;
}
