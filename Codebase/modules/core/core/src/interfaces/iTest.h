#pragma once

#include "cpch.h"
#include "cdefs.h"
#include "iRenderer.h"
#include "iInputSystem.h"


class Win32Window;
class MasterRenderer;

class iTest
{
public:
	iTest(const std::wstring& test_name, Win32Window* pWindow, MasterRenderer* pRenderer, std::map<InputType, iInputSystem*> inputSystems);
	virtual ~iTest() {};
	
	void OnInitillize();
	void OnInput();
	void OnUpdate(float deltaTime);
	void OnRender();
	void OnCleanup();
	bool Quit();
protected:
	virtual void Init() = 0;
	virtual void Input() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void LateUpdate(float fixedDeltaTime)=0;
	virtual void Destroy() = 0;

	MasterRenderer* renderer;
	std::map<InputType, iInputSystem*> input;
	Win32Window* window;
};