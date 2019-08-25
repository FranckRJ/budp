#include "textureManager.hpp"

namespace
{
    std::map<std::string, sf::Texture> mapOfTextures;
}

sf::Texture& textureManagerTool::getThisTexture(std::string textureName)
{
    std::map<std::string, sf::Texture>::iterator textureIte = mapOfTextures.find(textureName);

    if(textureIte == mapOfTextures.end())
    {
        sf::Texture newTexture;

        newTexture.loadFromFile(textureName);

        textureIte = mapOfTextures.emplace(textureName, std::move(newTexture)).first;
    }

    return textureIte->second;
}

void textureManagerTool::clearTexturesLoaded()
{
    mapOfTextures.clear();
}
