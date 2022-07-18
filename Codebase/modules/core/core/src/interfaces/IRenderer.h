#pragma once
#include "cpch.h"
#include "cdefs.h"
#include "iGameWindow.h"
#include "Mathematics.h"

namespace Graphics
{
	class iSurface;
}

enum class RenderType
{
	GUI,
	Debug,
	Forward,
	Deferred
};

#define RENDERER_DEBUG		0000'0001u
#define RENDERER_GUI		1000'0000u

class iRenderer
{
public:
	iRenderer() {}
	virtual ~iRenderer() {}

	virtual void OnInitilize(iGameWindow* pGameWindow) = 0;
	virtual void OnShutdown() = 0;
	
	virtual void OnRender() = 0;
	virtual void OnUpdate(double) = 0;
	virtual void UpdateViewMatrix(matrix4f _view) = 0;

	inline static iRenderer* Create(RenderType type);

	static std::map<RenderType, iRenderer*> CreateRenderers(uint mask);
};

#define DEFAULT_RENDERERS iRenderer::CreateRenderers(RENDERER_GUI | RENDERER_DEBUG)