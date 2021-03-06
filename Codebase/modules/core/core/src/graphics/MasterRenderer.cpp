#include "MasterRenderer.h"
#include "Commands.h"
#include "DebugRenderer.h"

using namespace Graphics;

MasterRenderer::MasterRenderer() 
	: pGameWindow {nullptr} 
{
}

MasterRenderer::~MasterRenderer()
{
	RenderCommands::Cleanup();
}

void MasterRenderer::Initilize(uint api, iGameWindow* pGameWindow)
{
	this->pGameWindow = pGameWindow;
	this->viewMatrix = Matrix::Identity;

	RenderCommands::SelectAPI((API)api, pGameWindow);

	renderers = DEFAULT_RENDERERS;
	for (const auto& entry : renderers)
	{
		auto r = entry.second;
		r->OnInitilize(pGameWindow);
	}
	// Setup Default Renderers 
}

void MasterRenderer::Shutdown()
{
	for (const auto& entry : renderers)
	{
		auto r = entry.second;
		r->OnShutdown();
		delete r;
	}
}

void MasterRenderer::Update(double deltaTime)
{
	renderers[RenderType::GUI]->OnUpdate(deltaTime);

	for (const auto& entry : renderers)
	{
		auto r = entry.second;
		if (entry.first != RenderType::GUI)
			r->OnUpdate(deltaTime);
	}
}

void MasterRenderer::Render()
{
	RenderCommands::BeginFrame();
	{	// Set Frame Clear Color
		RenderCommands::SetClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		// Set Frame Depth Value
		RenderCommands::SetDepth(1.0);
		// Clear The Current Frame
		RenderCommands::Clear(COLOR_BUFFER | DEPTH_BUFFER);

		// for each of the renderers: r
		// for each of r->pipelines
		// bind their pipeline
		// set their respective settings. 
		// 
		for (const auto& entry : renderers)
		{
			auto r = entry.second;
			if(entry.first != RenderType::GUI)
				r->OnRender();
		}
		
		renderers[RenderType::GUI]->OnRender();
	}

	RenderCommands::Present(false);
	RenderCommands::EndFrame();
}

void MasterRenderer::UpdateViewMatrix(matrix4f _view)
{
	for (const auto& entry : renderers)
	{
		auto r = entry.second;
		r->UpdateViewMatrix(_view);
	}
}

iRenderer* MasterRenderer::GetRenderer(RenderType type)
{
	return renderers[type];
}

MasterRenderer* MasterRenderer::Create(uint api, iGameWindow* pGameWindow)
{
	MasterRenderer* mr = new MasterRenderer();
	mr->Initilize(api, pGameWindow);
	return mr;
}