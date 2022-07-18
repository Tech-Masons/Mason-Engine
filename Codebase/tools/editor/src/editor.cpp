#include "Editor.h"
#include "interfaces/iTest.h"
#include "graphics/DebugRenderer.h"
#include "graphics/MasterRenderer.h"

Editor::Editor()
{
	window = new Win32Window("Mason Engine", 1280, 720);
	window->Open();

	masterRenderer = MasterRenderer::Create(RENDER_API, window);

	InputSystems = DEFAULT_INPUT_SYSTEMS;
	for (const auto& entry : InputSystems)
	{
		iInputSystem* i = entry.second;
	}

	window->AttachInputSystems(InputSystems);
}

Editor::~Editor()
{
}

uint64 Editor::Initilize()
{
	return 0;
}

uint64 Editor::Tick()
{
	// Create a View Matrix
	matrix4f camera = Matrix::Identity;
	matrix4f rotation = Matrix::RotationRollPitchYaw({ TO_RADIAN(30.0f),0.0f, 0.0f });
	camera = Matrix::RotateGlobal(camera, rotation);
	camera.row3 = { 0,5,-10,1.0 };
	camera = Matrix::Inverse(camera);

	// start the editor clock
	clock.Start();
	uint64_t eCode = 0;

	while (window->IsRunning())
	{
		window->Poll();
		if (!window->IsRunning())
		{
			eCode = 0;
			break;
		}

		// aquire deltaTime, elapsed time between Ticks
		auto deltaTime = clock.Signal();
		
		///SUBMIT ALL RENDER COMMANDS HERE -------------------------------------------------

		 //Aqure the Debug Renderer
		auto dbg = TypeCast<DebugRenderer>(masterRenderer->GetRenderer(RenderType::Debug));

		 //request a grid to be drawn by the debug renderer
		dbg->DrawGridXZ(10.0f, 0.5f, { 0.0f, 1.0f, 0.0f, 1.0f });
		dbg->DrawMatrix(Matrix::Identity);

		///-------------------------------------------------

		masterRenderer->Update(deltaTime);
		masterRenderer->UpdateViewMatrix(camera);
		
		masterRenderer->Render();
	}
	return 0;
}

uint64 Editor::Shutdown()
{
	/*for (auto& entry : Renderers) {
		iRenderer* renderer = entry.second;
		delete renderer;
		entry.second = nullptr;
	}
	Renderers.clear();*/
	delete masterRenderer;
	delete window;
	return 0;
}