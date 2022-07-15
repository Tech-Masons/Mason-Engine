#pragma once

#include "cpch.h"
#include "cdefs.h"

class Clock {
public:
	Clock();
	~Clock();


	void Start();
	double Signal();
	double GetElapsed();
	void Stop();

	std::chrono::high_resolution_clock::time_point T_START;
	std::chrono::high_resolution_clock::time_point T_CURRENT;

};