#ifndef TEXTUREMANAGER_HPP
#define TEXTUREMANAGER_HPP

#include <map>
#include <string>
#include <SFML/Graphics/Texture.hpp>

namespace textureManagerTool
{
    sf::Texture& getThisTexture(std::string textureName);
    void clearTexturesLoaded();
}

#endif
