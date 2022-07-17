#pragma once

#include "cpch.h"
#include "cdefs.h"
#include "interfaces/iInputSystem.h"
#include <bitset>

enum MouseButton {
	LMB, MMB, RMB
};

class Mouse : public iInputSystem 
{
public:
	Mouse();
	~Mouse();

	bool IsMouseButtonDown(MouseButton btn);
	std::pair<int, int> GetMousePosition();
	
	void OnMouseButtonDown(MouseButton btn);
	void OnMouseButtonUp(MouseButton btn);
	void OnMouseMove(float x, float y);

	bool TrackPosition();

	void EnableTracking();
	void DisableTracking();
private:
	std::bitset<10> mousebuttons;
	
	struct
	{
		int X;
		int Y;

		int mouse_x;
		int mouse_y;

		float delta_x;
		float delta_y;
	} state;

	bool enable_tracking;
};