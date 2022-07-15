#pragma once

#include "cpch.h"
#include "cdefs.h"

#include "interfaces/IRenderer.h"

class GUIRenderer : public iRenderer
{
public:
	virtual void OnInitilize(Graphics::iSurface*) override;
	virtual void OnShutdown() override;
	virtual void OnRender() override;
	virtual void OnUpdate(double) override;
	virtual void UpdateViewMatrix(matrix4f _view) override;

private:
	uint vb;

};

