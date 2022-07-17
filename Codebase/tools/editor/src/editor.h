#pragma once

#include <iostream>
#include "interfaces/IExecutable.h"
#include "interfaces/IRenderer.h"
#include "interfaces/iInputSystem.h"
#include "time/clock.h"

class Window;
class MasterRenderer;
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

	//std::map<RenderType, iRenderer*> Renderers;
	std::map<InputType, iInputSystem*> InputSystems;
private:
	Window* window;
	Clock clock;
};

iExecutable* CreateExecutable()
{
	return new Editor();
}