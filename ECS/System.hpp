#pragma once
#include <set>

#include "BaseTypes.hpp"

/*
 * System管理一组Entity，这些Entity具有相同的Components。System用来实现逻辑功能。
 *
 * 使用set管理这组Entity，而不是list的原因：
 * 删除的时间复杂度：set O(logn) (set是二叉树)  ;  list O(n)
 * 如果set中已经存在一个Entity，erase和insert时不会发生任何事情，省去了list先检查的步骤。
 *
 * System的运行模式：
 *  for (auto const& entity : mEntities) {
 *      auto& rigidBody = GetComponent<RigidBody>(entity);
 *      auto& transform = GetComponent<Transform>(entity);
 *      auto const& gravity = GetComponent<Gravity>(entity);
 *      
 *      // 依据System自身的功能，修改Entity相应的Component的数据
 *      transform.position += rigidBody.velocity * dt;
 *      rigidBody.velocity += gravity.force * dt;
 *  }
 * 
 */

class System {
public:
    std::set<EntityID> Entities_Set{};
};
