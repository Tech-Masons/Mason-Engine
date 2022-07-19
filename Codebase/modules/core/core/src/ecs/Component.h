#pragma once

#include "cpch.h"
#include "cdefs.h"

#include "mathematics.h"

struct Tag {
	std::string value;
};

struct PerspectiveCamera {
	float FieldOfView;
	float AspectRatio;
	float NearPlane;
	float FarPlane;

	matrix4f GetProjection() {
		return Matrix::ProjectionLH(FieldOfView, AspectRatio, NearPlane, FarPlane);
	}
	
};

struct Transform
{
	void SetPosition(float3 pos) {
		world.row3 = { pos.x, pos.y, pos.z, 1.0f };
		position = world.row3.xyz();
	}

	void SetRotation(float3 rot) {
		matrix4f tr = Matrix::RotationRollPitchYaw(rot);
		world = Matrix::RotateGlobal(world, tr);
		world.row3 = position;
		rotation = rotation;
	}

	float3 GetPosition() {
		return position;
	}

	float3 Right() { return  world.row0.xyz(); };
	float3 Up() { return world.row1.xyz(); };
	float3 Forward() { return world.row2.xyz(); };

	matrix4f& GetMatrix() {
		return world;
	}

private:
	matrix4f world = Matrix::Identity;

	float3 position;
	float3 rotation;
	float3 scale;

};
