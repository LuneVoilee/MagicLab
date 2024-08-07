#pragma once
#include <cassert>
#include <unordered_map>

#include "BaseTypes.hpp"

/*
 *  这个数据结构用来存放同一类型的Component的全部实例。
 *  ECS高性能的特性要求它必须是一个简单紧凑的数组。
 *
 *  如果直接把EntityID当作下标取实例，由于不知道Entity对应实例是否已经被销毁（ID是可以重新分配的，这个ID可能之前被销毁过），
 *  在获取前就必须先判断是否有效再获取，这样效率很低，也违背了紧凑的要求。
 *
 *
 *  解决方法：
 *  在Entity和数组实例之间建立索引，若Entity拥有该类型Component的实例，可以根据Entity找到对应数组实例的下标。
 *  当销毁某个Component实例时，把数组的最后一个实例移至销毁的实例的位置，并修改Entity对应的索引下标。
 *  由于这样做还需要通过原位置下标找到对应Entity，所以应该建立双向索引。
 *
 *
 *  我们还额外定义了一个IComponentArray接口，
 *  这是因为一个Entity可能拥有不同类型的Component，销毁一个Entity时，需要销毁不同类型的实例。
 *  使用虚函数多态是一种简单的处理方法，以后可以改用多播委托。
 *
 *
 *  为什么ECS把原本OOP中存储的数据拆成更小的Components，效率反而更高了？
 *  拆成很多小组件的好处是同样大小的Cache空间能够容纳更多的Component，
 *  即使ComponentArray中的组件顺序与Entiry的遍历顺序不一样，也会有一个很高的命中率。
 *  间接寻址也不是影响性能的主要因素，主要因素是命中率。
 *  而在一个System遍历第一个Entity的时候，相关的Components就已经调入Cache了，这时即使间接寻址也只是在Cache中寻址而已。
 *  相比起来OOP拿着指针在Heap中去寻找那些七零八落的Object，性能差太多了。
 *
 *
 *  
 */


class IComponentArray {
public:
    virtual ~IComponentArray();
    virtual void UpdateAfterEntityDestroyed(EntityID ID) = 0;
};

inline IComponentArray::~IComponentArray() = default;

template <typename ComponentType>
class ComponentArray : public IComponentArray {
public:
    void AddInstance(EntityID ID, ComponentType ComponentInstance) {
        assert(
            EntityToIndex_Map.find(ID)==EntityToIndex_Map.end() &&
            "For the same type of component, each entity can have at most one.");
        size_t EntityIndex = CurrentEntityNum;
        Component_Array[EntityIndex] = ComponentInstance;
        EntityToIndex_Map[ID] = EntityIndex;
        IndexToEntity_Map[EntityIndex] = ID;
        CurrentEntityNum++;
    }

    void DeleteInstance(EntityID ID) {
        assert(
            EntityToIndex_Map.find(ID)!=EntityToIndex_Map.end() &&
            "Can't find the instance when deleting a entity.");

        //获取正在删除的实例的下标
        size_t DeletingIndex = EntityToIndex_Map[ID];
        //获取数组最后一个实例的下标
        size_t LastIndex = EntityToIndex_Map[CurrentEntityNum - 1];
        //获取最后一个实例对应的Entity
        EntityID LastEntityID = IndexToEntity_Map[LastIndex];

        //直接用最后一个实例替换正在删除的实例，一步完成了删除和替换。
        Component_Array[DeletingIndex] = Component_Array[LastIndex];

        //修正索引：LastEntity <-> DeletingIndex
        EntityToIndex_Map[LastEntityID] = DeletingIndex;
        IndexToEntity_Map[DeletingIndex] = LastEntityID;

        //修正索引：LastIndex , ID 不再存在
        EntityToIndex_Map.erase(ID);
        IndexToEntity_Map.erase(LastIndex);

        CurrentEntityNum--;
    }

    ComponentType& GetInstance(EntityID ID) {
        assert(
            EntityToIndex_Map.find(ID)!=EntityToIndex_Map.end() &&
            "Can't find the instance when a entity getting its instance.");
        return Component_Array[ID];
    }

    void UpdateAfterEntityDestroyed(EntityID ID) override {
        if (EntityToIndex_Map.find(ID) != EntityToIndex_Map.end()) {
            DeleteInstance(ID);
        }
    }

private:
    //存放同一类型的Component的所有实例，一共最多只有MAX_ENTITIES个Entity。
    std::array<ComponentType, MAX_ENTITIES> Component_Array{};
    //每个EntityID对应一个Component实例（通过数组下标size_t联系起来）
    std::unordered_map<EntityID, size_t> EntityToIndex_Map{};
    //每个Component实例对应一个EntityID（通过数组下标size_t联系起来）
    std::unordered_map<size_t, EntityID> IndexToEntity_Map{};
    //已经分配过Component的Entity的数量（用于确定数组下标）
    size_t CurrentEntityNum{0};
};
