#pragma once

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOGDI
#define NOMINMAX
#include <Windows.h>
#include <Windowsx.h>
#else
#endif

#include <string>
#include <map>
#include <cstdint>