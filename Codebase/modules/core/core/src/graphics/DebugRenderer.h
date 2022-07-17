#pragma once
#include "cpch.h"
#include "cdefs.h"

#include "interfaces/iRenderer.h"
#include "structures/Vertices.h"

class DebugRenderer : public iRenderer
{
public:
	virtual void OnInitilize(Graphics::iSurface* pWindow) override;

	virtual void OnShutdown() override;

	virtual void OnRender() override;

	virtual void OnUpdate(double deltaTime) override;

	virtual void UpdateViewMatrix(matrix4f _view) override;
public:
	void AddLine(Line line, Color4 color);
	void AddLine(Line line, Color4 colorA, Color4 colorB);

	void DrawGridXZ(float size, float step, float4 color);
	void DrawMatrix(matrix4f matrix);

	void DrawColliderAABB(matrix4f matrix, float3 scale, Color4 color);

	void DrawColliderOBB(matrix4f matrix, Color4 color);

	void ClearLines();
private:
	LineList lines;
	uint pipeline_id;

	matrix4f viewMatrix;

	uint line_buffer;
	Graphics::iSurface* surface;
};