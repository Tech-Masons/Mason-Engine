#include "IRenderer.h"

#include "graphics/DebugRenderer.h"
#include "graphics/GUIRenderer.h"

iRenderer* iRenderer::Create(RenderType type)
{
	switch (type)
	{
	case RenderType::Debug:
		return new DebugRenderer();
	case RenderType::Forward:
		break;
	case RenderType::Deferred:
		break;

	case RenderType::GUI:
		return new GUIRenderer();
	default:
		break;
	}
}

std::map<RenderType, iRenderer*> iRenderer::CreateRenderers(uint mask)
{
	std::map<RenderType, iRenderer*> renderers;

	if ((mask & RENDERER_DEBUG) == RENDERER_DEBUG)
	{
		renderers[RenderType::Debug] = iRenderer::Create(RenderType::Debug);
	}

	if ((mask & RENDERER_GUI) == RENDERER_GUI)
	{
		renderers[RenderType::GUI] = iRenderer::Create(RenderType::GUI);
	}

	return renderers;
}
