#include "Editor.h"
#include "graphics/MasterRenderer.h"

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

		///-------------------------------------------------

		masterRenderer->Update(deltaTime);
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