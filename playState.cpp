#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/Color.hpp>

#include "playState.hpp"
#include "internalServer.hpp"
#include "externalServer.hpp"

playStateClass::playStateClass()
{
    server.reset(new internalServerClass);
}

void playStateClass::update(sf::RenderWindow& window)
{
    sf::Event event;

    while(window.pollEvent(event))
    {
        if(event.type == sf::Event::Closed)
        {
            window.close();
        }
        else if(event.type == sf::Event::KeyPressed)
        {
            if(event.key.code == sf::Keyboard::Left)
            {
                listOfDirection.push_front(directionEnum::left);
            }
            else if(event.key.code == sf::Keyboard::Right)
            {
                listOfDirection.push_front(directionEnum::right);
            }
            else if(event.key.code == sf::Keyboard::Up)
            {
                listOfDirection.push_front(directionEnum::up);
            }
            else if(event.key.code == sf::Keyboard::Down)
            {
                listOfDirection.push_front(directionEnum::down);
            }
        }
        else if(event.type == sf::Event::KeyReleased)
        {
            if(event.key.code == sf::Keyboard::Left)
            {
                listOfDirection.remove(directionEnum::left);
            }
            else if(event.key.code == sf::Keyboard::Right)
            {
                listOfDirection.remove(directionEnum::right);
            }
            else if(event.key.code == sf::Keyboard::Up)
            {
                listOfDirection.remove(directionEnum::up);
            }
            else if(event.key.code == sf::Keyboard::Down)
            {
                listOfDirection.remove(directionEnum::down);
            }
        }
    }

    if(listOfDirection.empty() == false)
    {
        server->setInputKey(listOfDirection.front());
    }
    else
    {
        server->setInputKey(directionEnum::nowhere);
    }

    server->update();
}

void playStateClass::draw(sf::RenderWindow& window)
{
    window.clear(sf::Color(200, 200, 200));
    server->draw(window);
}
