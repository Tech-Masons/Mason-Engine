#include "Editor.h"
#include "graphics/MasterRenderer.h"
#include "graphics/DebugRenderer.h"

Editor::Editor()
{
	window = new Win32Window("Mason Engine", 1280, 720);
	window->Open();

	masterRenderer = MasterRenderer::Create(RENDER_API, window);

	InputSystems = DEFAULT_INPUT_SYSTEMS;
	window->AttachInputSystems(InputSystems);
}

Editor::~Editor()
{
}

uint64 Editor::Initilize()
{
	// editor camera entity
	return 0;
}

uint64 Editor::Tick()
{
	matrix4f camera = Matrix::Identity;
	matrix4f rotation = Matrix::RotationRollPitchYaw({ TO_RADIAN(30.0f), 0.0f, 0.0f });
	camera = Matrix::RotateGlobal(camera, rotation);
	camera.row3 = { 0, 5, -10, 1.0 };
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

		auto dbg = TypeCast<DebugRenderer>(masterRenderer->GetRenderer(RenderType::Debug));
		dbg->DrawGridXZ(25, 1, { 1.0f, 0.0f, 0.0f, 1.0f });

		///-------------------------------------------------

		masterRenderer->Update(deltaTime);
		masterRenderer->UpdateViewMatrix(camera);
		masterRenderer->Render();
	}
	return 0;
}

uint64 Editor::Shutdown()
{
	delete masterRenderer;
	delete window;
	return 0;
}