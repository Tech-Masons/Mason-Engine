#pragma once

#include "cpch.h"
#include "cdefs.h"
#include <Mathematics.h>

struct Entity
{
	matrix4f world;
public:
	void SetPosition(float3 pos) { world.row3 = { pos.x, pos.y, pos.z, world.row3.w }; }
	float3 Position() { return world.row3.xyz(); }
	float3 Right() { return world.row0.xyz(); }
	float3 Up() { return world.row1.xyz(); }
	float3 Forward() { return world.row2.xyz(); }
};