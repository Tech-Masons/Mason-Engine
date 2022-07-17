#pragma once
#include "cpch.h"
#include "cdefs.h"

enum class InputType
{
	Mouse,
	Keyboard
};

#define KEYBOARD_AND_MOUSE	0x0000'0000'0000'0001u

class iInputSystem
{
public:
	iInputSystem() {}
	virtual ~iInputSystem() {}

	static iInputSystem* CreateInputSystem(InputType type);
	static std::map<InputType, iInputSystem*> CreateInputSystems(uint mask);
};

#define DEFAULT_INPUT_SYSTEMS iInputSystem::CreateInputSystems(KEYBOARD_AND_MOUSE);