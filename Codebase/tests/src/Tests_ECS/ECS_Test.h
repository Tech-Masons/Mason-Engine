#pragma once

#include "interfaces/iTest.h"
#include "Win32Window.h"
#include "graphics/MasterRenderer.h"

class Scene;

class ECS_Test : public iTest
{
public:
	ECS_Test(Win32Window* pWindow, MasterRenderer* renderers, std::map<InputType, iInputSystem*> inputSystems);

	virtual void Init() override;
	virtual void Input() override;
	virtual void Update(float deltaTime) override;
	virtual void LateUpdate(float fixedDeltaTime) override;
	virtual void Destroy() override;

	void HandleEditorCameraControl(double deltaTime);

private:
	Scene* LoadScene();


	// Test Scene Functionality 
	Scene* scene;

};

