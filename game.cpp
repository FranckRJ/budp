#include <ctime> // std::time()
#include <cstdlib> // std::srand()
//#include <SFML/Window/ContextSettings.hpp> //bug lors de l'inclusion de cette classe
#include <SFML/Window/VideoMode.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Sleep.hpp>

#include "game.hpp"
#include "playState.hpp"
#include "serverState.hpp"
#include "global.hpp"

gameClass::gameClass()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    window.create(sf::VideoMode(WIDTH_SCREEN, HEIGHT_SCREEN), "Bomberman UDP", sf::Style::Titlebar | sf::Style::Close, settings);
    window.setFramerateLimit(60);
    //window.setVerticalSyncEnabled(true);

    std::srand(std::time(NULL));

    global::activeGameStateStack = &gameStateStack;
    gameStateStack.set(new playStateClass);
}

void gameClass::run()
{
    sf::Clock clock;
    sf::Time elapsedTime;
    sf::Time differenceTime;

    while(window.isOpen())
    {
        elapsedTime = clock.getElapsedTime();

        do
        {
            gameStateStack.setChange(false);
            gameStateStack.update(window);
        }
        while(gameStateStack.getChange() == true);

        gameStateStack.draw(window);
        window.display();

        differenceTime = clock.getElapsedTime() - elapsedTime;
        if(differenceTime.asMilliseconds() < 1000 / 60)
        {
            sf::sleep(sf::milliseconds((1000 / 60) - differenceTime.asMilliseconds()));
        }
    }
}
