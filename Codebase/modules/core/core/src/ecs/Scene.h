#pragma once
#include "Entity.h"
#include "ComponentPool.h"
#include "Component.h"

class Scene {

public:
	Scene() {}
	~Scene() {}

	EntityID CreateEntity(const std::string& name = "Entity");

	void DestroyEntity(EntityID id);

	template<typename T>
	T* AddComponent(EntityID id)
	{
		// if the entity has already been deleted just 
		// return from here
		if (entities[Entity::GetEntityIndex(id)].id != id)
			return nullptr;

		int compID = Entity::GetComponentID<T>();

		// the pool is not big enough. needs to be resized
		if (pools.size() <= compID)
		{
			pools.resize(compID + 1, nullptr);
		}

		if (pools[compID] == nullptr)
		{
			pools[compID] = new ComponentPool(sizeof(T));
		}

		T* pComponent = new (pools[compID]->Get(Entity::GetEntityIndex(id))) T();

		entities[Entity::GetEntityIndex(id)].components.set(compID);

		return pComponent;
	}

	template<typename T>
	T* GetComponent(EntityID id)
	{
		// if the entity has already been deleted just 
		// return from here
		if (entities[Entity::GetEntityIndex(id)].id != id)
			return nullptr;

		int compID = Entity::GetComponentID<T>();

		if (!entities[Entity::GetEntityIndex(id)].components.test(compID)) {
			return nullptr;
		}

		T* pComp = static_cast<T*>(pools[compID]->Get(Entity::GetEntityIndex(id)));

		return pComp;

	}

	template<typename T>
	void RemoveComponent(EntityID id)
	{
		// if the entity has already been deleted just 
		// return from here
		if (entities[Entity::GetEntityIndex(id)].id != id)
			return;


		int compID = GetId<T>();
		entities[Entity::GetEntityIndex(id)].components.reset(compID);
	}

	std::vector<Entity> entities;
	std::vector<EntityIndex> free;
	std::vector<ComponentPool*> pools;
};

template<typename... CTypes>
struct ComponentView
{
	ComponentView(Scene* _scene) : scene{ _scene }
	{
		if (sizeof...(CTypes) == 0)
		{
			all = true;
		}
		else
		{
			// unpack template params
			int cpIDS[] = { 0, Entity::GetComponentID<CTypes>() ... };
			for (int i = 1; i < (sizeof...(CTypes) + 1); i++)
			{
				components.set(cpIDS[i]);
			}
		}
	}

	struct Iterator {
		Iterator(Scene* pScene, EntityIndex idx, ComponentMask cpts, bool _all)
			: scene{ pScene }, index{ idx }, components{ cpts }, all{ _all }
		{
		}
		
		EntityID operator*() const
		{
			return scene->entities[index].id;
		}

		bool operator==(const Iterator& other) const
		{
			return index == other.index || index == scene->entities.size();
		}

		bool operator!=(const Iterator& other) const
		{
			return index != other.index || index != scene->entities.size();
		}

		Iterator& operator++()
		{
			do {
				index++;
			} while (index < scene->entities.size() && !ValidIndex());

			return *this;
		}

		bool ValidIndex()
		{
			return
				// is the entity valid?
				Entity::IsEntityValid(scene->entities[index].id) &&
				// and is the component mask valid?
				(all || components == (components & scene->entities[index].components));
		}

		uint index;
		Scene* scene;
		ComponentMask components;
		bool all { false };
	};

	const Iterator begin() const
	{
		int first = 0;

		while (first < scene->entities.size() &&
			(components != (components & scene->entities[first].components)
				|| !Entity::IsEntityValid(scene->entities[first].id)))
		{
			first++;
		}

		return Iterator(scene, first, components, all);
	}

	const Iterator end() const
	{
		return Iterator(scene, EntityIndex(scene->entities.size()), components, all);
	}

	Scene* scene { nullptr };
	ComponentMask components;
	bool all { false };
};