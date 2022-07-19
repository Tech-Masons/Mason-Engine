#include "iTest.h"

#include "Win32Window.h"
#include "graphics/MasterRenderer.h"

iTest::iTest(const std::wstring& test_name, Win32Window* pWindow, MasterRenderer* pRenderer, std::map<InputType, iInputSystem*> inputSystems)
	: window{ pWindow }, renderer{ pRenderer }, input{inputSystems}
{
}

bool iTest::Quit()
{
	return false;
}

void iTest::OnInitillize()
{
	Init();
}

void iTest::OnInput()
{
	Input();
}

void iTest::OnUpdate(float deltaTime)
{
	Update(deltaTime);
}

void iTest::OnRender()
{	
}

void iTest::OnCleanup()
{
}