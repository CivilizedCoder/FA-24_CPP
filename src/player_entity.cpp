#include <iostream>
#include "raylib.h"
#include "player_entity.h"
#include "world.h"

#include <iostream>

PlayerEntity::PlayerEntity(float x, float y, int width, int height, EntityType type)
	: Entity(x, y, width, height, type)
{
	PubSub::subscribe("action", this);

	movementCountdown = 0;
	targetX = 0;
	targetY = 0;
	speedX = 0;
	speedY = 0;
	flip = false;
}

void PlayerEntity::tick()
{
	if (movementCountdown > 0)
	{
		movementCountdown--;

		if (movementCountdown == 0)
		{
			setMoving(false);
			setX(targetX);
		}
		else
		{
			setX(getX() + speedX);
		}
	}
	if(flip == false)
		speedY += 0.3;
	else 
		speedY -= 0.3;
	

	setY(getY() + speedY);
	setFalling(true);

	Vector2 position = { getX(), getY() };
	PubSub::publish("player", "location", &position);
}

void PlayerEntity::receiveMessage(string channel, string message, void* data)
{
	if (channel == "action" && message == "player")
	{
		PlayerAction* action = (PlayerAction*)data;

		if (*action == PlayerRight)
		{
			setMoving(true);
			setFacing(Right);

			targetX = getX() + speedX;
			targetY = getY();
			speedX = 1;
			movementCountdown = 2;
			
		}

		if (*action == PlayerLeft)
		{
			setMoving(true);
			setFacing(Left);
			speedX = -1;
			targetX = getX() + speedX;
			movementCountdown = 2;

		}

		if (*action == PlayerUp)
		{
			if (flip == false)
				speedY -= 0.5;
			else
				speedY += 0.5;

		}
		if (*action == PlayerJump) {
			flip = !flip;
		}

	}
}

bool PlayerEntity::handleCollisions()
{
	for (Entity* entity : collisions)
	{
		if (entity->getType() == Obstacle)
		{
			int xDistance = abs(getX() - entity->getX());
			int yDistance = abs(getY() - entity->getY());

			if (xDistance > yDistance)
			{
				setMoving(false);
				movementCountdown = 0;

				setX(getX() - speedX);
			}
			else
			{
				if (getFalling())
				{
					setFalling(false);
					setY(getY() - speedY);
					speedY = 0;
				}
			}
		}
		if (entity->getType() == Bounce) {
			if (!flip)
				speedY = -12;
			else
				speedY = 12;
			
		}
		if (entity->getType() == Hurt) {
			targetX = 320;
			targetY = 280;
			setX(320);
			setY(280);
			speedY = 0;
		}
		if (entity->getType() == Goal) {
			//Game Over

		}
	}

	collisions.clear();
	return false;
}
