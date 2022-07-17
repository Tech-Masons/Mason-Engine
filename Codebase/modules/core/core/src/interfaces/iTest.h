#pragma once

#include "cpch.h"
#include "cdefs.h"
#include "iRenderer.h"
#include "iInputSystem.h"

class Window;

class iTest
{
public:
	iTest(const std::wstring& test_name);
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

	std::map<RenderType, iRenderer*> Renderers;
	std::map<InputType, iInputSystem*> InputSystems;
private:
	Window* window;
};