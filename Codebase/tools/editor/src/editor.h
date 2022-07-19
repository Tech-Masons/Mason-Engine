#pragma once

#include <iostream>
#include "interfaces/IExecutable.h"
#include "interfaces/IRenderer.h"
#include "Win32Window.h"
#include "iInputSystem.h"
#include "time/clock.h"
#include "StartupConfig.h"

class Window;
class MasterRenderer;
class Scene;
class iTest;

class Editor : public iExecutable
{
public:
	Editor();
	~Editor();

	virtual uint64 Initilize() override;
	virtual uint64 Tick() override;
	virtual uint64 Shutdown() override;
private:
	MasterRenderer* masterRenderer;

	std::map<InputType, iInputSystem*> InputSystems;
private:
	Win32Window* window;
	Clock clock;
};

#if EXE_Editor
iExecutable* CreateExecutable()
{
	return new Editor();
}
#endif