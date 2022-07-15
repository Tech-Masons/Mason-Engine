#include "Keyboard.h"

bool Keyboard::IsKeyPressed(KeyCode kc)
{
	return (keys[kc] == true);
}

void Keyboard::OnKeyDown(KeyCode kc)
{
	// Dispatch Event OnKeyPressed(keyCode)
	keys[kc] = true;
	// used to determine what virtual windows keycode matches to what FSE keycode
	//std::cout << kc << std::endl;

}

void Keyboard::OnKeyUp(KeyCode kc)
{
	// Dispatch Event OnKeyReleased(kc)
	keys[kc] = false;
}
