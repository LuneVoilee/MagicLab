#pragma once
#include <array>
#include <cassert>

#include "BaseTypes.hpp"
#include <queue>

class EntityManager {
public:
    EntityManager() {
        //初始化AvailableEntityID_Queue，刚开始所有ID都是可用的
        for (EntityID id = 0; id < MAX_ENTITIES; id++) {
            AvailableEntityID_Queue.push(id);
        }
    }

    //分配新的ID并更新队列和计数
    EntityID CreateEntity() {
        assert(CurrentEntitiesNum < MAX_ENTITIES && "Entity has reach its max value");
        EntityID newID = AvailableEntityID_Queue.front();
        AvailableEntityID_Queue.pop();
        CurrentEntitiesNum++;
        return newID;
    }

    //清除Signature，更新队列和计数
    void DestroyEntity(EntityID oldID) {
        assert(oldID < MAX_ENTITIES && "This ID is illegal");
        Signature_Array[oldID].reset();
        AvailableEntityID_Queue.push(oldID);
        CurrentEntitiesNum--;
    }

    //设置指定Entity的Signature
    void SetSignature(EntityID ID, Signature signature) {
        assert(ID < MAX_ENTITIES && "This ID is illegal");
        Signature_Array[ID] = signature;
    }

    //获取指定Entity的Signature
    Signature GetSignature(EntityID ID) {
        assert(ID < MAX_ENTITIES && "This ID is illegal");
        return Signature_Array[ID];
    }

private:
    //存放还没有使用的EntityID
    std::queue<EntityID> AvailableEntityID_Queue{};
    //存放每个Entity的Signature
    std::array<Signature, MAX_ENTITIES> Signature_Array{};
    //目前已经拥有的Entity数量
    unsigned int CurrentEntitiesNum{0};
};
