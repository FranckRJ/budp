#include <typeinfo>

template <class TYPE>
TYPE* entityClass::getComponent()
{
    std::map<std::type_index, std::unique_ptr<componentBase>>::iterator iteComponentMap = mapOfComponents.find(typeid(TYPE));

    if(iteComponentMap != mapOfComponents.end())
    {
        return static_cast<TYPE*>(iteComponentMap->second.get());
    }
    else
    {
        return nullptr;
    }
}

template <class TYPE>
entityClass& entityClass::addComponent(TYPE* newComponent)
{
    if(mapOfComponents.find(typeid(TYPE)) == mapOfComponents.end())
    {
        mapOfComponents.emplace(typeid(TYPE), std::unique_ptr<componentBase>(newComponent));
    }
    else
    {
        delete newComponent;
    }

    return *this;
}
