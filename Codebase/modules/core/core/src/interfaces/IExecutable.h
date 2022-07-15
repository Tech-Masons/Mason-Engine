#pragma once

typedef long long unsigned uint64;
#include <iostream>


class iExecutable {
public:
	iExecutable() = default;
	virtual ~iExecutable() = default;

	virtual uint64 Initilize() = 0;
	virtual uint64 Tick() = 0;
	virtual uint64 Shutdown() = 0;

	virtual uint64 Run() {
		uint64 result = 0;
		result = Initilize();
		result = Tick();
		result = Shutdown();
		return result;
	};


};


iExecutable* CreateExecutable();
