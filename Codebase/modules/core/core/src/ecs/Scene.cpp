#include "Scene.h"

EntityID Scene::CreateEntity(const std::string& name)
{
	if (!free.empty())
	{
		EntityIndex newIDX = free.back();
		free.pop_back();

		EntityID newID = Entity::CreateEntityID(newIDX, Entity::GetEntityVersion(entities[newIDX].id));
		entities[newIDX].id = newID;

		return entities[newID].id;
	}

	Entity entity = { Entity::CreateEntityID(EntityIndex(entities.size()), 0), ComponentMask() };
	entities.push_back(entity);

	Transform* cam_trans = AddComponent<Transform>(entity.id);
	Tag* cam_tag = AddComponent<Tag>(entity.id);
	cam_tag->value = name;

	return entities.back().id;
}

void Scene::DestroyEntity(EntityID id)
{
	EntityID newID = Entity::CreateEntityID(EntityIndex(-1), Entity::GetEntityVersion(id) + 1);
	entities[Entity::GetEntityIndex(id)].id = newID;
	entities[Entity::GetEntityIndex(id)].components.reset();

	free.push_back(Entity::GetEntityIndex(id));
}