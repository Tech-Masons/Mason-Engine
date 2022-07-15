#pragma once

#ifdef _WIN32
#define CENTER_SCREEN_X(width) (GetSystemMetrics(SM_CXMAXIMIZED) / 2) - (width / 2)
#define CENTER_SCREEN_Y(height) (GetSystemMetrics(SM_CYMAXIMIZED) / 2) - (height / 2)
#endif

#define contains_flag(bitstr, flag) (bitstr & flag) == flag
#define set_flag_on(bitstr, flag) bitstr |= flag
#define toggle_flag(bitstr, flag) bitstr &= ~(flag)

typedef unsigned int uint;