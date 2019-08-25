#ifndef ENTITYBUILDER_HPP
#define ENTITYBUILDER_HPP

#include <string>
#include <vector>
#include <SFML/System/Vector2.hpp>

#include "entity.hpp"

namespace entityBuilderTool
{
    entityClass buildEntity(std::string entityName);
    entityClass buildEntity(std::string entityName, sf::Vector2i basePositionIfNeeded);
    void fillEntityListBasedOnLevel(const std::vector<std::uint8_t>& levelContent, std::vector<entityClass>& thisList);
    void fillEntityListBasedOnLevel(const std::string& levelContent, std::vector<entityClass>& thisList);
    std::vector<std::uint8_t> getLevelContentInCBOR(std::string levelName);
}

#endif
