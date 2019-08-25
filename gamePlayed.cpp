#include <utility> //std::move, std::pair
#include <fstream> //std::ifstream
#include <sstream> //std::stringstream
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

#include "gamePlayed.hpp"
#include "entityBuilder.hpp"
#include "global.hpp"

//TODO trier la liste des entity au lieu de créer une rendermap
void gamePlayedClass::loadLevelFromFile(std::string levelName)
{
    std::ifstream file("levels/" + levelName + ".json");
    std::stringstream buffer;
    buffer << file.rdbuf();
    entityBuilderTool::fillEntityListBasedOnLevel(buffer.str(), listOfEntitys);
}

void gamePlayedClass::loadLevelFromCBOR(const std::vector<std::uint8_t>& levelContent)
{
    entityBuilderTool::fillEntityListBasedOnLevel(levelContent, listOfEntitys);
}

void gamePlayedClass::update()
{
    mapOfRenderedEntitys.clear();

    for(entityClass& thisEntity : listOfEntitys)
    {
        processEntityInput(thisEntity);
        moveEntityAndProcessCollision(thisEntity);
        updateEntityPosition(thisEntity);
        updateEntityAnimation(thisEntity);
        addToRenderedEntitysMap(thisEntity);
    }
}

void gamePlayedClass::draw(sf::RenderWindow& window)
{
    for(std::pair<const int, entityClass*>& thisEntity : mapOfRenderedEntitys)
    {
        componentRender* renderComponent = thisEntity.second->getComponent<componentRender>();

        if(renderComponent != nullptr)
        {
            window.draw(renderComponent->sprite);
        }
    }
}

int gamePlayedClass::addPlayer()
{
    listOfEntitys.push_back(entityBuilderTool::buildEntity("player", sf::Vector2i(35, 35)));

    return listOfEntitys.size() - 1;
}

void gamePlayedClass::setInputOfThisEntity(int idOfEntity, directionEnum direction)
{
    componentInput* inputComponent = listOfEntitys[idOfEntity].getComponent<componentInput>();

    if(inputComponent != nullptr)
    {
        inputComponent->goLeft = false;
        inputComponent->goRight = false;
        inputComponent->goUp = false;
        inputComponent->goDown = false;

        if(direction == directionEnum::left)
        {
            inputComponent->goLeft = true;
        }
        else if(direction == directionEnum::right)
        {
            inputComponent->goRight = true;
        }
        else if(direction == directionEnum::up)
        {
            inputComponent->goUp = true;
        }
        else if(direction == directionEnum::down)
        {
            inputComponent->goDown = true;
        }
    }
}

void gamePlayedClass::processEntityInput(entityClass& thisEntity)
{
    componentInput* inputComponent = thisEntity.getComponent<componentInput>();

    if(inputComponent != nullptr)
    {
        componentMovable* movableComponent = thisEntity.getComponent<componentMovable>();
        componentDirection* directionComponent = thisEntity.getComponent<componentDirection>();

        if(movableComponent != nullptr)
        {
            movableComponent->velocity.x = 0;
            movableComponent->velocity.y = 0;

            if(directionComponent != nullptr)
            {
                directionComponent->isWalking = false;
            }

            if(inputComponent->goUp == true && inputComponent->goDown == false)
            {
                movableComponent->velocity.y = -movableComponent->speed;

                if(directionComponent != nullptr)
                {
                    directionComponent->direction = directionEnum::up;
                    directionComponent->isWalking = true;
                }
            }
            else if(inputComponent->goDown == true && inputComponent->goUp == false)
            {
                movableComponent->velocity.y = movableComponent->speed;

                if(directionComponent != nullptr)
                {
                    directionComponent->direction = directionEnum::down;
                    directionComponent->isWalking = true;
                }
            }

            if(inputComponent->goLeft == true && inputComponent->goRight == false)
            {
                movableComponent->velocity.x = -movableComponent->speed;

                if(directionComponent != nullptr)
                {
                    directionComponent->direction = directionEnum::left;
                    directionComponent->isWalking = true;
                }
            }
            else if(inputComponent->goRight == true && inputComponent->goLeft == false)
            {
                movableComponent->velocity.x = movableComponent->speed;

                if(directionComponent != nullptr)
                {
                    directionComponent->direction = directionEnum::right;
                    directionComponent->isWalking = true;
                }
            }
        }
    }
}

void gamePlayedClass::moveEntityAndProcessCollision(entityClass& thisEntity)
{
    componentMovable* movableComponent = thisEntity.getComponent<componentMovable>();
    componentPosition* positionComponent = thisEntity.getComponent<componentPosition>();

    if(movableComponent != nullptr && positionComponent != nullptr)
    {
        componentCollideBox* collideBoxComponent = thisEntity.getComponent<componentCollideBox>();

        positionComponent->position += movableComponent->velocity;

        if(collideBoxComponent != nullptr)
        {
            if(positionComponent->position.x < 0)
            {
                positionComponent->position.x = 0;
            }
            else if(positionComponent->position.x + collideBoxComponent->collideBoxSize.x > WIDTH_SCREEN)
            {
                positionComponent->position.x = WIDTH_SCREEN - collideBoxComponent->collideBoxSize.x;
            }

            if(positionComponent->position.y < 0)
            {
                positionComponent->position.y = 0;
            }
            else if(positionComponent->position.y + collideBoxComponent->collideBoxSize.y > HEIGHT_SCREEN)
            {
                positionComponent->position.y = HEIGHT_SCREEN - collideBoxComponent->collideBoxSize.y;
            }

            processCollisionWithOthersEntitys(thisEntity);
        }
    }
}

void gamePlayedClass::processCollisionWithOthersEntitys(entityClass& thisEntity)
{
    componentPosition* positionComponent = thisEntity.getComponent<componentPosition>();
    componentCollideBox* collideBoxComponent = thisEntity.getComponent<componentCollideBox>();
    componentMovable* movableComponent = thisEntity.getComponent<componentMovable>();

    if(positionComponent != nullptr && collideBoxComponent != nullptr && movableComponent != nullptr)
    {
        for(entityClass& secondEntity : listOfEntitys)
        {
            if(&thisEntity != &secondEntity)
            {
                componentPosition* secondPosition = secondEntity.getComponent<componentPosition>();
                componentCollideBox* secondCollideBox = secondEntity.getComponent<componentCollideBox>();

                if(secondCollideBox != nullptr && secondPosition != nullptr)
                {
                    if(secondCollideBox->isSolid == true)
                    {
                        if(collideBoxIntersects(*positionComponent, *collideBoxComponent, *secondPosition, *secondCollideBox) == true)
                        {
                            if(movableComponent->velocity.x > 0)
                            {
                                positionComponent->position.x = secondPosition->position.x - collideBoxComponent->collideBoxSize.x;
                            }
                            else if(movableComponent->velocity.x < 0)
                            {
                                positionComponent->position.x = secondPosition->position.x + secondCollideBox->collideBoxSize.x;
                            }

                            if(movableComponent->velocity.y > 0)
                            {
                                positionComponent->position.y = secondPosition->position.y - collideBoxComponent->collideBoxSize.y;
                            }
                            else if(movableComponent->velocity.y < 0)
                            {
                                positionComponent->position.y = secondPosition->position.y + secondCollideBox->collideBoxSize.y;
                            }
                        }
                    }
                }
            }
        }
    }
}

void gamePlayedClass::updateEntityPosition(entityClass& thisEntity)
{
    componentPosition* positionComponent = thisEntity.getComponent<componentPosition>();

    if(positionComponent != nullptr)
    {
        componentRender* renderComponent = thisEntity.getComponent<componentRender>();

        if(renderComponent != nullptr)
        {
            renderComponent->sprite.setPosition(positionComponent->position.x + renderComponent->marginOfSprite.x,
                                                positionComponent->position.y + renderComponent->marginOfSprite.y);
        }
    }
}

void gamePlayedClass::updateEntityAnimation(entityClass& thisEntity)
{
    componentAnimation* animationComponent = thisEntity.getComponent<componentAnimation>();
    componentRender* renderComponent = thisEntity.getComponent<componentRender>();

    if(animationComponent != nullptr && renderComponent != nullptr)
    {
        componentDirection* directionComponent = thisEntity.getComponent<componentDirection>();
        directionEnum directionOfSprite = directionEnum::left;
        bool isWalking = false;
        animationStruct* currentAnimation;

        if(directionComponent != nullptr)
        {
            directionOfSprite = directionComponent->direction;
            isWalking = directionComponent->isWalking;
        }

        currentAnimation = &(animationComponent->animations[static_cast<int>(directionOfSprite) + (isWalking == true ? 4 : 0)]);

        if(currentAnimation->listOfSprites.size() > 0)
        {
            sf::Vector2i currentSprite;
            currentAnimation->spriteDurationLeft -= 1;

            if(currentAnimation->spriteDurationLeft <= 0)
            {
                currentAnimation->spriteDurationLeft = currentAnimation->spriteDuration;
                currentAnimation->currentSpriteNumber += 1;

                if(currentAnimation->currentSpriteNumber >= static_cast<int>(currentAnimation->listOfSprites.size()))
                {
                    currentAnimation->currentSpriteNumber = 0;
                }
            }

            currentSprite = currentAnimation->listOfSprites[currentAnimation->currentSpriteNumber];

            renderComponent->sprite.setTextureRect(sf::IntRect(currentSprite.x, currentSprite.y,
                                                               renderComponent->spriteSize.x, renderComponent->spriteSize.y));
        }
    }
}

void gamePlayedClass::addToRenderedEntitysMap(entityClass& thisEntity)
{
    componentRender* renderComponent = thisEntity.getComponent<componentRender>();

    if(renderComponent != nullptr)
    {
        if(renderComponent->forceYRenderPos == false)
        {
            mapOfRenderedEntitys.emplace(renderComponent->sprite.getPosition().y + renderComponent->sprite.getTextureRect().height,
                                         &thisEntity);
        }
        else
        {
            mapOfRenderedEntitys.emplace(renderComponent->yRenderPos, &thisEntity);
        }
    }
}

bool gamePlayedClass::collideBoxIntersects(componentPosition& firstPos, componentCollideBox& firstCollideBox, componentPosition& secondPos, componentCollideBox& secondCollideBox) const
{
    return (firstPos.position.x < (secondPos.position.x + secondCollideBox.collideBoxSize.x) &&
       (firstPos.position.x + firstCollideBox.collideBoxSize.x) > secondPos.position.x &&
       firstPos.position.y < (secondPos.position.y + secondCollideBox.collideBoxSize.y) &&
       (firstPos.position.y + firstCollideBox.collideBoxSize.y) > secondPos.position.y);
}
