#pragma once

#include "cpch.h"
#include "cdefs.h"
#include <Mathematics.h>

#include "ECSLimits.h"

struct Entity {
	EntityID id;
	ComponentMask components;


	static int component_id_counter;

	template<class T>
	static int GetComponentID()
	{
		static int id = component_id_counter++;
		return id;
	}

	inline static EntityID CreateEntityID(EntityIndex index, EntityVersion version) {
		return (((EntityID)index << 32) | (EntityID)version);
	}

	inline static EntityIndex GetEntityIndex(EntityID id) {
		return id >> 32;
	}

	inline static EntityVersion GetEntityVersion(EntityID id) {
		return (EntityVersion)id;
	}

	inline static bool IsEntityValid(EntityID id)
	{
		return (id >> 32) != EntityIndex(-1);
	}

};



