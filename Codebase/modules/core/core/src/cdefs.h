#pragma once

typedef unsigned int uint;

#ifdef _DEBUG
	#include "debug/logger.h"

	#define LOG(msg) Logger::Log(msg)

	#define THROW(msg) LOG(msg); throw std::runtime_error(msg)

	// Memory Leak Detection
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <stdint.h>
	#include <crtdbg.h>
	#include <sstream>

	#define LEAK_FLAG -1

	#define CATCH_MEM_LEAKS _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF )
	#define DUMP_MEM_LEAKS _CrtDumpMemoryLeaks()
	#define FIND_MEMORY_LEAK _CrtSetBreakAlloc(LEAK_FLAG)
#else 
	#define LOG
	#define LEAK_FLAG

	#define CATCH_MEM_LEAKS 
	#define DUMP_MEM_LEAKS 
	#define MEM_LEAK_SET_OUTPUT 

	#define CHECK
#endif

#define START_WINDOW_SIZE_X 1280
#define START_WINDOW_SIZE_Y  720

#define OPENGL 0
#define DIRECTX11 1
#define DIRECTX12 2
#define VULKAN 3
#define RENDER_API DIRECTX12

// downcast interface members
template<typename T>
static T* TypeCast(void* ptr) { return reinterpret_cast<T*>(ptr); }