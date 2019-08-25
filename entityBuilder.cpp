#include <utility> //std::move
#include <fstream> //std::ifstream
#include <SFML/Graphics/Rect.hpp>
#include <json/json.hpp>

#include "entityBuilder.hpp"
#include "component.hpp"

using json = nlohmann::json;

namespace
{
    int getJsonInt(const json& thisJson, int valueIfNotInt)
    {
        if(thisJson.is_number())
        {
            return thisJson.get<int>();
        }
        else
        {
            return valueIfNotInt;
        }
    }

    bool getJsonBool(const json& thisJson, bool valueIfNotBool)
    {
        if(thisJson.is_boolean())
        {
            return thisJson.get<bool>();
        }
        else
        {
            return valueIfNotBool;
        }
    }

    std::string getJsonString(const json& thisJson, std::string valueIfNotString)
    {
        if(thisJson.is_string())
        {
            return thisJson.get<std::string>();
        }
        else
        {
            return valueIfNotString;
        }
    }

    componentPosition* jsonToPositionComponent(json& currentJson)
    {
        if(currentJson.is_object() == true)
        {
            int posX = getJsonInt(currentJson["posX"], 0);
            int posY = getJsonInt(currentJson["posY"], 0);

            return new componentPosition(sf::Vector2i(posX, posY));
        }
        else
        {
            return new componentPosition();
        }
    }

    componentRender* jsonToRenderComponent(json& currentJson)
    {
        if(currentJson.is_object())
        {
            json jsonFileName = currentJson["file"];
            if(jsonFileName.is_string())
            {
                int marginX = getJsonInt(currentJson["marginX"], 0);
                int marginY = getJsonInt(currentJson["marginY"], 0);
                int subTextureX = getJsonInt(currentJson["subTextureX"], -1);
                int subTextureY = getJsonInt(currentJson["subTextureY"], -1);
                int subTextureW = getJsonInt(currentJson["subTextureW"], -1);
                int subTextureH = getJsonInt(currentJson["subTextureH"], -1);
                bool forceYRender = getJsonBool(currentJson["forceYRender"], false);
                int yRenderPos = getJsonInt(currentJson["yRenderPos"], 0);

                return componentTool::makeRenderComponent(jsonFileName.get<std::string>(), sf::Vector2i(marginX, marginY),
                                                          sf::IntRect(subTextureX, subTextureY, subTextureW, subTextureH),
                                                          forceYRender, yRenderPos);
            }
        }
        return nullptr;
    }

    componentCollideBox* jsonToCollideBoxComponent(json& currentJson)
    {
        if(currentJson.is_object() == true)
        {
            int sizeX = getJsonInt(currentJson["sizeX"], 0);
            int sizeY = getJsonInt(currentJson["sizeY"], 0);
            bool isSolid = getJsonBool(currentJson["isSolid"], true);

            return new componentCollideBox(sf::Vector2i(sizeX, sizeY), isSolid);
        }
        else
        {
            return new componentCollideBox();
        }
    }

    componentMovable* jsonToMovableComponent(json& currentJson)
    {
        if(currentJson.is_object() == true)
        {
            int speed = getJsonInt(currentJson["speed"], 0);

            return new componentMovable(speed);
        }
        else
        {
            return new componentMovable();
        }
    }

    componentDirection* jsonToDirectionComponent(json& currentJson)
    {
        if(currentJson.is_object() == true)
        {
            json jsonDefaultDirection = currentJson["defaultDirection"];
            directionEnum defaultDirection = directionEnum::left;

            if(jsonDefaultDirection.is_string() == true)
            {
                std::string stringDefaultDirection = jsonDefaultDirection.get<std::string>();
                if(stringDefaultDirection == "down")
                {
                    defaultDirection = directionEnum::down;
                }
                else if(stringDefaultDirection == "left")
                {
                    defaultDirection = directionEnum::left;
                }
                else if(stringDefaultDirection == "up")
                {
                    defaultDirection = directionEnum::up;
                }
                else if(stringDefaultDirection == "right")
                {
                    defaultDirection = directionEnum::right;
                }
            }

            return new componentDirection(defaultDirection);
        }
        else
        {
            return new componentDirection();
        }
    }

    void fillAnimationStructWithJson(animationStruct& animationToFill, json& currentJson)
    {
        if(currentJson.is_object() == true)
        {
            json imagesJson = currentJson["images"];

            if(imagesJson.is_array())
            {
                for(json& currentImageJson : imagesJson)
                {
                    if(currentImageJson.is_object())
                    {
                        int posX = getJsonInt(currentImageJson["posX"], 0);
                        int posY = getJsonInt(currentImageJson["posY"], 0);

                        animationToFill.listOfSprites.push_back(sf::Vector2i(posX, posY));
                    }
                }
            }

            animationToFill.spriteDuration = getJsonInt(currentJson["imageDuration"], 999999);
        }
    }

    componentAnimation* jsonToAnimationComponent(json& currentJson)
    {
        componentAnimation* animationComponent = new componentAnimation;

        if(currentJson.is_object() == true)
        {
            fillAnimationStructWithJson(animationComponent->animations[0], currentJson["standLeft"]);
            fillAnimationStructWithJson(animationComponent->animations[1], currentJson["standRight"]);
            fillAnimationStructWithJson(animationComponent->animations[2], currentJson["standUp"]);
            fillAnimationStructWithJson(animationComponent->animations[3], currentJson["standDown"]);
            fillAnimationStructWithJson(animationComponent->animations[4], currentJson["walkLeft"]);
            fillAnimationStructWithJson(animationComponent->animations[5], currentJson["walkRight"]);
            fillAnimationStructWithJson(animationComponent->animations[6], currentJson["walkUp"]);
            fillAnimationStructWithJson(animationComponent->animations[7], currentJson["walkDown"]);
        }

        return animationComponent;
    }

    void fillEntityListBasedOnLevelInJson(const json& jsonLevel, std::vector<entityClass>& thisList)
    {
        if(jsonLevel.is_object() == true)
        {
            json jsonEntityList = jsonLevel["entities"];

            if(jsonEntityList.is_array() == true)
            {
                for(json currentEntity : jsonEntityList)
                {
                    if(currentEntity.is_object() == true)
                    {
                        std::string name = getJsonString(currentEntity["name"], "");
                        if(name.empty() == false)
                        {
                            json jsonPosX = currentEntity["posX"];
                            json jsonPosY = currentEntity["posY"];

                            if(jsonPosX.is_number() || jsonPosY.is_number())
                            {
                                int posX = getJsonInt(jsonPosX, 0);
                                int posY = getJsonInt(jsonPosY, 0);
                                thisList.push_back(entityBuilderTool::buildEntity(name, sf::Vector2i(posX, posY)));
                            }
                            else
                            {
                                thisList.push_back(entityBuilderTool::buildEntity(name));
                            }
                        }
                    }
                }
            }
        }
    }
}

entityClass entityBuilderTool::buildEntity(std::string entityName)
{
    entityClass newEntity;
    json jsonContent;
    json currentComponent;

    try
    {
        std::ifstream file("entities/" + entityName + ".json");
        file >> jsonContent;
    }
    catch(...)
    {
        jsonContent = nullptr;
    }

    if(jsonContent.is_object() == true)
    {
        currentComponent = jsonContent["componentPosition"];
        if(currentComponent.is_null() == false)
        {
            newEntity.addComponent(jsonToPositionComponent(currentComponent));
        }

        currentComponent = jsonContent["componentRender"];
        if(currentComponent.is_null() == false)
        {
            componentRender* newRenderComponent = jsonToRenderComponent(currentComponent);
            if(newRenderComponent != nullptr)
            {
                newEntity.addComponent(newRenderComponent);
            }
        }

        currentComponent = jsonContent["componentCollideBox"];
        if(currentComponent.is_null() == false)
        {
            newEntity.addComponent(jsonToCollideBoxComponent(currentComponent));
        }

        currentComponent = jsonContent["componentInput"];
        if(currentComponent.is_null() == false)
        {
            newEntity.addComponent(new componentInput);
        }

        currentComponent = jsonContent["componentMovable"];
        if(currentComponent.is_null() == false)
        {
            newEntity.addComponent(jsonToMovableComponent(currentComponent));
        }

        currentComponent = jsonContent["componentDirection"];
        if(currentComponent.is_null() == false)
        {
            newEntity.addComponent(jsonToDirectionComponent(currentComponent));
        }

        currentComponent = jsonContent["componentAnimation"];
        if(currentComponent.is_null() == false)
        {
            newEntity.addComponent(jsonToAnimationComponent(currentComponent));
        }
    }

    return newEntity;
}

entityClass entityBuilderTool::buildEntity(std::string entityName, sf::Vector2i basePosition)
{
    entityClass newEntity = buildEntity(entityName);
    componentPosition* positionComponent = newEntity.getComponent<componentPosition>();

    if(positionComponent != nullptr)
    {
        positionComponent->position = basePosition;
    }

    return newEntity;
}

void entityBuilderTool::fillEntityListBasedOnLevel(const std::vector<std::uint8_t>& levelContent, std::vector<entityClass>& thisList)
{
    json jsonLevel;

    try
    {
        jsonLevel = json::from_cbor(levelContent);
    }
    catch(...)
    {
        jsonLevel = nullptr;
    }

    fillEntityListBasedOnLevelInJson(jsonLevel, thisList);
}

void entityBuilderTool::fillEntityListBasedOnLevel(const std::string& levelContent, std::vector<entityClass>& thisList)
{
    json jsonLevel;

    try
    {
        jsonLevel = json::parse(levelContent);
    }
    catch(...)
    {
        jsonLevel = nullptr;
    }

    fillEntityListBasedOnLevelInJson(jsonLevel, thisList);
}

std::vector<std::uint8_t> entityBuilderTool::getLevelContentInCBOR(std::string levelName)
{
    json jsonLevel;

    try
    {
        std::ifstream file("levels/" + levelName + ".json");
        file >> jsonLevel;
    }
    catch(...)
    {
        jsonLevel = nullptr;
    }

    return json::to_cbor(jsonLevel);
}
