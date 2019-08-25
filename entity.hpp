#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <map>
#include <typeindex>
#include <memory> //std::unique_ptr
#include <string>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

#include "component.hpp"

class entityClass
{
public:
    entityClass() = default;
    entityClass(entityClass&& other) = default;
    template <class TYPE>
    TYPE* getComponent();
    template <class TYPE>
    entityClass& addComponent(TYPE* newComponent);
private:
    std::map<std::type_index, std::unique_ptr<componentBase>> mapOfComponents;
};

#include "entity.tpp"

#endif
