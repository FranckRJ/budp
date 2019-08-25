#include "component.hpp"
#include "textureManager.hpp"

componentRender* componentTool::makeRenderComponent(std::string fileName, sf::Vector2i newMarginOfSprite,
                                                    sf::IntRect subTexture, bool newForceYRenderPos, int newYRenderPos)
{
    componentRender* renderComponent = new componentRender;

    renderComponent->sprite.setTexture(textureManagerTool::getThisTexture(fileName));
    renderComponent->marginOfSprite = newMarginOfSprite;
    renderComponent->forceYRenderPos = newForceYRenderPos;
    renderComponent->yRenderPos = newYRenderPos;

    if(subTexture.left != -1)
    {
        renderComponent->sprite.setTextureRect(subTexture);
        renderComponent->spriteSize.x = subTexture.width;
        renderComponent->spriteSize.y = subTexture.height;
    }
    else
    {
        renderComponent->spriteSize.x = renderComponent->sprite.getGlobalBounds().width;
        renderComponent->spriteSize.y = renderComponent->sprite.getGlobalBounds().height;
    }

    return renderComponent;
}
