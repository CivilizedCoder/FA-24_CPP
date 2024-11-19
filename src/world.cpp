#include <iostream>
#include <cstdlib>
#include <ctime>
#include "world.h"
#include "player_entity.h"
#include "pubsub.h"
#include "raylib.h"

World::World()
{
	srand(time(NULL));
}

void World::tick()
{
	for (Entity *entity : entities)
		entity->tick();

	// Detect collisions
	collisions.clear();
	for (Entity* entity : entities)
	{
		if (entity->getMoving() || entity->getFalling())
		{
			for (Entity* entity2 : entities)
			{
				if (entity != entity2)
				{
					checkCollision(entity, entity2);
				}
			}
		}
	}

	// Handle collisions
	deletions.clear();
	for (Entity* entity : collisions)
	{
		if (entity->handleCollisions())
			deletions.push_back(entity);
	}

	// Remove deleted entities
	for (Entity* entity : deletions)
	{
		vector<Entity*>::iterator result = find(entities.begin(), entities.end(), entity);

		if (result != entities.end())
			entities.erase(result);

		PubSub::publish("entity", "delete", entity);
		delete entity;
	}


}

void World::checkCollision(Entity* entity, Entity* entity2)
{
	Rectangle rec1 = {entity->getX(), entity->getY(), entity->getWidth(), entity->getHeight()};
	Rectangle rec2 = { entity2->getX(), entity2->getY(), entity2->getWidth(), entity2->getHeight() };

	if (CheckCollisionRecs(rec1, rec2))
	{
		entity->addCollision(entity2);

		if (find(collisions.begin(), collisions.end(), entity) == collisions.end())
			collisions.push_back(entity);

		if (!entity2->getMoving())
		{
			entity2->addCollision(entity);
			if (find(collisions.begin(), collisions.end(), entity2) == collisions.end())
				collisions.push_back(entity2);
		}
	}
}

void World::clearWorld()
{
	for (Entity* entity : entities)
	{
		// Let everyone know we're deleting this entity
		PubSub::publish("entity", "delete", entity);
		delete entity;
	}

	entities.clear();
}

void World::addEntity(int x, int y, int width, int height, EntityType type)
{
	Entity* entity = new Entity(x, y, width, height, type);

	// Let everyone know the entity has been created
	PubSub::publish("entity", "new", entity);

	entities.push_back(entity);
}

void World::addPlayer(int x, int y, int width, int height, EntityType type)
{
	Entity* entity = new PlayerEntity(x, y, width, height, type);

	// Let everyone know the entity has been created
	PubSub::publish("entity", "new", entity);

	entities.push_back(entity);
}

