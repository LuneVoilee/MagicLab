#pragma once
#include <memory>
#include <unordered_map>

#include "BaseTypes.hpp"
#include "ComponentArray.hpp"

//管理不同类型的Component数组

class ComponentManager {
public:
    template <typename ComponentType>
    void RegisterComponent() {
        const char* ComponentName = typeid(ComponentType).name();
        assert(
            NameToComponentID_Map.find(ComponentName)==NameToComponentID_Map.end() &&
            "You can't register the same component again");
        NameToComponentID_Map[ComponentName] = CurrentComponentID;
        NameToComponentArray_Map[ComponentName] = std::make_shared<ComponentArray<ComponentType>>();
        CurrentComponentID++;
    }

    template <typename ComponentType>
    ComponentID GetComponentID() {
        const char* ComponentName = typeid(ComponentType).name();
        assert(
            NameToComponentID_Map.find(ComponentName)!=NameToComponentID_Map.end() &&
            "This register is not registed");
        return NameToComponentID_Map[ComponentName];
    }

    template <typename ComponentType>
    void AddComponent(EntityID ID, ComponentType Instance) {
        GetComponentArray<ComponentType>()->AddInstance(ID, Instance);
    }

    template <typename ComponentType>
    void DeleteComponent(EntityID ID) {
        GetComponentArray<ComponentType>()->DeleteInstance(ID);
    }

    template <typename ComponentType>
    void GetComponent(EntityID ID) {
        GetComponentArray<ComponentType>()->GetInstance(ID);
    }

    //删除一个Entity，意味着在所有的Component数组中进行查找并删除
    void DestroyEntity(EntityID ID) {
        for (const auto& pair : NameToComponentArray_Map) {
            const auto& ComponentArrayPtr = pair.second;
            ComponentArrayPtr->DestroyEntity(ID);
        }
    }

private:
    std::unordered_map<const char*, ComponentID> NameToComponentID_Map{};
    std::unordered_map<const char*, std::shared_ptr<IComponentArray>> NameToComponentArray_Map = {};
    ComponentID CurrentComponentID{0};


    template <typename ComponentType>
    std::shared_ptr<ComponentArray<ComponentType>> GetComponentArray() {
        const char* typeName = typeid(ComponentType).name();

        assert(
            NameToComponentArray_Map.find(typeName) != NameToComponentArray_Map.end() &&
            "Component not registered before use.");

        //static_pointer_cast创建了一个shared_ptr的新实例，共享所有权。
        return std::static_pointer_cast<ComponentArray<ComponentType>>(NameToComponentArray_Map[typeName]);
    }
};
