#pragma once

#include "cpch.h"
#include "cdefs.h"

#include "interfaces/iInputSystem.h"

#include <bitset>

enum KeyCode {
	LEFT_ARROW = 37, UP_ARROW,  RIGHT_ARROW, DOWN_ARROW,
	A = 65, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

};

class Keyboard : public iInputSystem
{
public:
	Keyboard(){}
	~Keyboard(){}
	static constexpr uint8_t KeyCount = 0xFFu;

	bool IsKeyPressed(KeyCode kc);
	
	void OnKeyDown(KeyCode kc);
	
	void OnKeyUp(KeyCode kc);


private:
	std::bitset<KeyCount> keys;
};

