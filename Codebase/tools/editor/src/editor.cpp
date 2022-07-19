#include "Editor.h"

#include "interfaces/iTest.h"
#include "graphics/MasterRenderer.h"

#include "TestSuite.h"

#include "StartupConfig.h"

iExecutable* CreateExecutable()
{
#if EXE_Editor
	return new Editor();
#elif EXE_TestSuite
	return new TestSuite();
#endif

}

extern iTest* CreateTest(Win32Window* pWindow, MasterRenderer* renderer, std::map<InputType, iInputSystem*> inputSystems);

Editor::Editor()
{
	window = new Win32Window("Mason Engine", 1280, 720);
	window->Open();

	masterRenderer = MasterRenderer::Create(RENDER_API, window);

	InputSystems = DEFAULT_INPUT_SYSTEMS;
	window->AttachInputSystems(InputSystems);

	
	test = CreateTest(window, masterRenderer, InputSystems);

}


Editor::~Editor()
{
}

uint64 Editor::Initilize()
{
	// editor camera entity

	test->OnInitillize();

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

		test->OnInput();
		test->OnUpdate(deltaTime);
		

		///-------------------------------------------------

		masterRenderer->Update(deltaTime);
		masterRenderer->Render();
	}
	return 0;
}

uint64 Editor::Shutdown()
{
	test->OnCleanup();

	delete masterRenderer;
	delete window;

	return 0;
}

