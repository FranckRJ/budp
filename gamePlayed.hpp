#ifndef GAMEPLAYED_HPP
#define GAMEPLAYED_HPP

#include <vector>
#include <string>
#include <map> //std::multimap
#include <SFML/Graphics/RenderWindow.hpp>

#include "entity.hpp"
#include "component.hpp"

class gamePlayedClass
{
public:
    void loadLevelFromFile(std::string levelName);
    void loadLevelFromCBOR(const std::vector<std::uint8_t>& levelContent);
    void update();
    void draw(sf::RenderWindow& window);
    int addPlayer();
    void setInputOfThisEntity(int idOfEntity, directionEnum direction);
private:
    void processEntityInput(entityClass& thisEntity);
    void moveEntityAndProcessCollision(entityClass& thisEntity);
    void processCollisionWithOthersEntitys(entityClass& thisEntity);
    void updateEntityPosition(entityClass& thisEntity);
    void updateEntityAnimation(entityClass& thisEntity);
    void addToRenderedEntitysMap(entityClass& thisEntity);
    bool collideBoxIntersects(componentPosition& firstPos, componentCollideBox& firstCollideBox,
                              componentPosition& secondPos, componentCollideBox& secondCollideBox) const;
private:
    std::vector<entityClass> listOfEntitys;
    std::multimap<int, entityClass*> mapOfRenderedEntitys;
};

#endif
