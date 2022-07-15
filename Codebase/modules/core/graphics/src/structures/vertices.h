#pragma once

#include "gfxpch.h"
#include "gfxdefs.h"

#include <vector>
#include <map>

#include "Mathematics.h"

struct DLine_Vertex {
	float4 xyzw;
	float4 color;
};

typedef	std::pair<float3, float3> Line;
typedef float4 Color4;

struct LineList {
	std::vector<DLine_Vertex> data;
public:
	void AddLine(Line line, Color4 color) {
		AddLine(line, color, color);
	}

	void AddLine(Line line, Color4 colorA, Color4 colorB) {

		// extract points
		float3 pointA = line.first;
		float3 pointB = line.second;

		DLine_Vertex Vertex0, Vertex1;
		Vertex0.xyzw = { pointA.x, pointA.y, pointA.z, 1.0 };
		Vertex0.color = colorA;

		Vertex1.xyzw = { pointB.x, pointB.y, pointB.z, 1.0 };
		Vertex1.color = colorB;

		data.push_back(Vertex0);
		data.push_back(Vertex1);

	}

	void ClearLines() {
		data.clear();
	}

};

struct BatchDesc {
	uint element_count;		
	uint* pStrides;

};


struct SceneData {
	matrix4f worldMatrix;		// 64
	matrix4f viewMatrix;		// 64
	matrix4f projectionMatrix;	// 64
	
	float4 padding[4];			// 64
};