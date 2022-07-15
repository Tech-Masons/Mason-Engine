#pragma once

#include "interfaces/iTest.h"

#define TEST_0 0
#define TEST_1 1
#define TEST_2 0

iTest* CreateTest()
{
#if TEST_0
#elif TEST_1
#elif TEST_2
#endif
	return nullptr;
}