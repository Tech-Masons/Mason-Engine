#pragma once

#include "cpch.h"
#include "cdefs.h"
#include <map>
#include "interfaces/iSurface.h"
#include "interfaces/iInputSystem.h"

class Window : public Graphics::iSurface
{
public:
	Window(const std::wstring& _title, int _width, int _height);
	~Window();

	virtual LRESULT CALLBACK MessageHandler(UINT msg, WPARAM w, LPARAM l) override;

	void AttachInputSystems(std::map<InputType, iInputSystem*>& _systems);
private:
	std::map<InputType, iInputSystem*> systems;
};