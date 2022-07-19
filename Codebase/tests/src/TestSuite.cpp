#include "TestSuite.h"

#include "interfaces/iTest.h"
#include "graphics/MasterRenderer.h"

extern iTest* CreateTest(Win32Window* pWindow, MasterRenderer* renderer, std::map<InputType, iInputSystem*> inputSystems);

TestSuite::TestSuite()
{
	window = new Win32Window("Test Suite", 1280, 720);
	window->Open();

	masterRenderer = MasterRenderer::Create(RENDER_API, window);

	InputSystems = DEFAULT_INPUT_SYSTEMS;
	window->AttachInputSystems(InputSystems);


	test = CreateTest(window, masterRenderer, InputSystems);

}


TestSuite::~TestSuite()
{
}

uint64 TestSuite::Initilize()
{
	// TestSuite camera entity

	test->OnInitillize();

	return 0;
}

uint64 TestSuite::Tick()
{


	// start the TestSuite clock
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

		std::string title = window->GetTitle();
		window->SetTitle("FPS: " + std::to_string((1000 / deltaTime) / 1000.0f));
		
		test->OnInput();
		test->OnUpdate(deltaTime);


		///-------------------------------------------------

		masterRenderer->Update(deltaTime);
		masterRenderer->Render();
	}
	return 0;
}

uint64 TestSuite::Shutdown()
{
	test->OnCleanup();

	delete masterRenderer;
	delete window;

	return 0;
}

