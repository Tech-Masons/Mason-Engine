#include "clock.h"

Clock::Clock()
{
}

Clock::~Clock()
{
}

void Clock::Start()
{
	T_START = std::chrono::high_resolution_clock::now();
}

double Clock::Signal()
{
	T_CURRENT = std::chrono::high_resolution_clock::now();
	double elapsed = GetElapsed();
	T_START = T_CURRENT;
	return elapsed;
}

double Clock::GetElapsed()
{
	return (T_CURRENT - T_START).count() * 1e-9;
}

void Clock::Stop()
{
	T_CURRENT = std::chrono::high_resolution_clock::now();
}
