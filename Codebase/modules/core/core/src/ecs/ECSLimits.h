#pragma once

#include <bitset>

typedef unsigned int EntityIndex;
typedef unsigned int EntityVersion;

typedef unsigned long long EntityID;

#define ECS_LIMITS_MAX_CONCURRET_COMPONENTS 64
#define ECS_LIMITS_MAX_CONCURRET_ENTITIES 1'000'000

typedef std::bitset<ECS_LIMITS_MAX_CONCURRET_COMPONENTS> ComponentMask;

