#pragma once
#include "PlatformDefines.h"

enum class InputType
{
	Mouse,
	Keyboard
};

#define KEYBOARD_AND_MOUSE	0x0000'0000'0000'0001u

class iInputSystem
{
public:
	iInputSystem() { }
	virtual ~iInputSystem() { }

	static iInputSystem* CreateInputSystem(InputType type);
	static std::map<InputType, iInputSystem*> CreateInputSystems(uint32_t mask);
};

#define DEFAULT_INPUT_SYSTEMS iInputSystem::CreateInputSystems(KEYBOARD_AND_MOUSE);