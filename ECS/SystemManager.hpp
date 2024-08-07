#pragma once
#include <cassert>
#include <memory>
#include <unordered_map>
#include "BaseTypes.hpp"
#include "System.hpp"

class SystemManager {
public:
    //注册System
    template <typename SystemType>
    std::shared_ptr<SystemType> RegisterSystem() {
        const char* SystemName = typeid(SystemType).name();
        assert(System_Map.find(SystemName) == System_Map.end() && "System has been registed");
        auto SystemPtr = std::make_shared<SystemType>();
        System_Map[SystemName] = SystemPtr;
        return SystemPtr;
    }

    //设置这个System接受哪些组件
    template <typename SystemType>
    void SetSignature(Signature signature) {
        const char* SystemName = typeid(SystemType).name();
        assert(System_Map.find(SystemName) == System_Map.end() && "System must be registed before used");
        Signature_Map[SystemName] = signature;
    }

    //每个Entity都有一个独特的ID，它可能出现在多个System里，所以需要遍历所有System删除。
    //对于不包含这个ID的System，调用erase什么也不会发生。
    //不同的Entity的签名也有可能是一样的，所以删除一个Entity时不应对Signature进行任何操作。
    void DestroyEntity(EntityID ID) {
        for (auto& pair : System_Map) {
            const auto& SystemPtr = pair.second;
            SystemPtr->Entities_Set.erase(ID);
        }
    }

    //当一个Entity的Signature发生变化的时候，在所有的System中进行更新，判断这个Entity属于哪些System。
    void EntitySignatureChanged(EntityID ID, Signature newSignature) {
        for (auto& pair : System_Map) {
            auto SystemName = pair.first;
            auto SystemPtr = pair.second;
            auto SystemNeededSignature = Signature_Map[SystemName];

            if ((SystemNeededSignature & newSignature) == SystemNeededSignature) {
                SystemPtr->Entities_Set.insert(ID);
            }
            else {
                SystemPtr->Entities_Set.erase(ID);
            }
        }
    }

private:
    //存放所有的Signature,每个SystemType对应一个Signature，
    //Signature标明这个System接受的Entity的类型（包含了哪些Component）。
    std::unordered_map<const char*, Signature> Signature_Map{};
    //存放所有的System
    std::unordered_map<const char*, std::shared_ptr<System>> System_Map{};
};
