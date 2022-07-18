#include "Mouse.h"

Mouse::Mouse()
{
}

Mouse::~Mouse()
{
}

bool Mouse::IsMouseButtonDown(MouseButton btn)
{
	return (mousebuttons[btn] == true);
}

std::pair<int, int> Mouse::GetMousePosition()
{
	return std::pair<int, int>(state.X, state.Y);
}

void Mouse::OnMouseButtonDown(MouseButton btn)
{
	// Dispatch Event OnMouseButtonPressed(int x, int y);
	mousebuttons[btn] = true;
}

void Mouse::OnMouseButtonUp(MouseButton btn)
{
	// Dispatch Event OnMouseButtonRelease(int x, int y);
	mousebuttons[btn] = false;
}

void Mouse::OnMouseMove(float x, float y)
{
	state.X = x;
	state.Y = y;
	
}

bool Mouse::TrackPosition()
{
	return enable_tracking;
}

void Mouse::EnableTracking()
{
	enable_tracking = true;
}

void Mouse::DisableTracking()
{
	enable_tracking = false;
}
