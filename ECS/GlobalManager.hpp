#pragma once
#include <memory>

#include "ComponentManager.hpp"
#include "EntityManager.hpp"
#include "SystemManager.hpp"

class GlobalManager {
public:
    void Init() {
        EntityManager_Ptr = std::make_unique<EntityManager>();
        ComponentManager_Ptr = std::make_unique<ComponentManager>();
        SystemManager_Ptr = std::make_unique<SystemManager>();
    }


    //Entity
    EntityID CreateEntity() {
        return EntityManager_Ptr->CreateEntity();
    }

    void DestroyEntity(EntityID ID) {
        EntityManager_Ptr->DestroyEntity(ID);
        ComponentManager_Ptr->UpdeateAfterEntityDestroyed(ID);
        SystemManager_Ptr->UpdateAfterEntityDestroyed(ID);
    }


    //Component
    template <typename ComponentType>
    void RegisterComponent() {
        ComponentManager_Ptr->RegisterComponent<ComponentType>();
    }

    template <typename ComponentType>
    void AddComponent(EntityID ID, ComponentType ComponentInstance) {
        ComponentManager_Ptr->AddComponent<ComponentType>(ID, ComponentInstance);

        auto TempSignature = EntityManager_Ptr->GetSignature(ID);
        TempSignature.set(ComponentManager_Ptr->GetComponentID<ComponentType>(), true);
        EntityManager_Ptr->SetSignature(ID, TempSignature);

        SystemManager_Ptr->EntitySignatureChanged(ID, TempSignature);
    }

    template <typename ComponentType>
    void DeleteComponent(EntityID ID) {
        ComponentManager_Ptr->DeleteComponent<ComponentType>(ID);

        auto TempSignature = EntityManager_Ptr->GetSignature(ID);
        TempSignature.set(ComponentManager_Ptr->GetComponentID<ComponentType>(), false);
        EntityManager_Ptr->SetSignature(ID, TempSignature);

        SystemManager_Ptr->EntitySignatureChanged(ID, TempSignature);
    }

    template <typename ComponentType>
    ComponentType& GetComponent(EntityID ID) {
        return ComponentManager_Ptr->GetComponent<ComponentType>(ID);
    }

    template <typename ComponentType>
    ComponentID GetComponentID() {
        return ComponentManager_Ptr->GetComponentID<ComponentType>();
    }


    //System
    template <typename SystemType>
    std::shared_ptr<SystemType> RegisterSystem() {
        return SystemManager_Ptr->RegisterSystem<SystemType>();
    }

    template <typename SystemType>
    void SetSystemSignature(Signature signature) {
        SystemManager_Ptr->SetSignature<SystemType>(signature);
    }

private:
    std::unique_ptr<EntityManager> EntityManager_Ptr;
    std::unique_ptr<ComponentManager> ComponentManager_Ptr;
    std::unique_ptr<SystemManager> SystemManager_Ptr;
};
