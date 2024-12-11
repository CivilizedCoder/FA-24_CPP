#ifndef PLAYERENTITY_H
#define PLAYERENTITY_H

#include "entity.h"
#include "pubsub.h"

class PlayerEntity : public Entity, public Subscriber
{
private:
	float targetX;
	float targetY;
	float speedX;
	float speedY;
	int movementCountdown;
	bool flip;
	int fuel;
	int grounded;

public:
	PlayerEntity(float x, float y, int width, int height, EntityType type);

	void tick();
	void receiveMessage(string channel, string message, void* data);
	bool handleCollisions();
};

#endif
