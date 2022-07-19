#pragma once


#include "StartupConfig.h"

#include "Tests_ECS/ECS_Test.h"


iTest* CreateTest(Win32Window* pWindow, MasterRenderer* pRenderer, std::map<InputType, iInputSystem*> inputSystems)
{
#if TEST_0
	return new ECS_Test(pWindow, pRenderer, inputSystems);
#elif TEST_1
#elif TEST_2
#else
	return nullptr;
#endif
}