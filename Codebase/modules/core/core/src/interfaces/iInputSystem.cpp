#include "iInputSystem.h"

#include "input/Keyboard.h"
#include "input/Mouse.h"

iInputSystem* iInputSystem::CreateInputSystem(InputType type)
{
	switch (type)
	{
	case InputType::Mouse:
		return new Mouse();
	case InputType::Keyboard:
		return new Keyboard();
	}

	return nullptr;
}

std::map<InputType, iInputSystem*> iInputSystem::CreateInputSystems(uint mask)
{
	std::map<InputType, iInputSystem*> systems;

	if ((mask & KEYBOARD_AND_MOUSE) == KEYBOARD_AND_MOUSE)
	{
		systems[InputType::Keyboard] = iInputSystem::CreateInputSystem(InputType::Keyboard);
		systems[InputType::Mouse] = iInputSystem::CreateInputSystem(InputType::Mouse);
	}


	return systems;
}