#pragma once
#include "cpch.h"
#include "cdefs.h"

#include <Mathematics.h>

struct Entity {
	matrix4f world;
public:
	void set_position(float3 pos) { world.row3 = { pos.x, pos.y, pos.z, world.row3.w }; }
	float3 position() { return world.row3.xyz(); }
	float3 right() { return world.row0.xyz(); }
	float3 up() { return world.row1.xyz(); }
	float3 forward() { return world.row2.xyz(); }
};