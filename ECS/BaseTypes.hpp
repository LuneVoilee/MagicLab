#pragma once
#include <bitset>

//Entity
//每个EntityID标识一个唯一的Entity。
using EntityID = unsigned int;
constexpr EntityID MAX_ENTITIES = 1000;

//Component
//每个Component标识一个唯一的Component。
using ComponentID = unsigned int;
constexpr ComponentID MAX_COMPONENTS = 32;

//Signature
//每个Entity都拥有一个Signature，用于标明这个Entity所含有的Component。
//例如有一个ComponentID为13的Component，把bitset的第13位设为1即可。
using Signature = std::bitset<MAX_COMPONENTS>;
