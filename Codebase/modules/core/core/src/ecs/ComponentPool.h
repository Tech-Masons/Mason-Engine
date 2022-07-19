#pragma once

#include "ECSLimits.h"

struct ComponentPool {
	ComponentPool(size_t size) {
		element_size = size;
		data = new char[element_size * ECS_LIMITS_MAX_CONCURRET_ENTITIES];
	}
	~ComponentPool() {
		delete[] data;
	}

	inline void* Get(size_t index) {
		return data + index * element_size;
	}

	char* data{ nullptr };
	size_t element_size{ 0 };
};

