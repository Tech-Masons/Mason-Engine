#include "iTest.h"
#include "graphics/DebugRenderer.h"

iTest::iTest(const std::wstring& test_name)
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