#include <iostream>
#include "raylib.h"
#include "player_entity.h"
#include "world.h"
#include <random>

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
	fuel = 300;
	grounded = 0;


}

void PlayerEntity::tick()
{
	
	grounded = 1;
	


	if (getX() == 320 && getY() > 317 && getY() < 321) {
		string text = "'W' to fly \n\n'A' and 'D' to move.\n\n'SPACE' to flip gravity.";
		DrawText(text.c_str(), 10, 350, 30, BLACK);
	}




			
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
	string* d = static_cast<string*>(data);
	if (channel == "action" && message == "player" && *d == ("refresh")) {
		string tank = "Fuel:" + to_string(fuel);
		DrawText(tank.c_str(), 10, 10, 20, BLACK);
	}


	if (channel == "action" && message == "player")
	{
		PlayerAction* action = (PlayerAction*)data;

		
		if (!(getX() == 160 && getY() == 192)) {
			if (*action == PlayerRight)
			{
				setMoving(true);
				
				if (flip)
					setFacing(Up);
				
				else
					setFacing(Right);


				targetX = getX() + speedX;
				targetY = getY();
				speedX = 2;
				movementCountdown = 2;

			}

			if (*action == PlayerLeft)
			{
				setMoving(true);
				if (flip)
					setFacing(Down);

				else
					setFacing(Left);
				speedX = -2;
				targetX = getX() + speedX;
				movementCountdown = 2;

			}
			if (*action == PlayerReset) {
				targetX = 320;
				targetY = 280;
				setX(320);
				setY(280);
				fuel = 300;
				speedY = 0;
			}

			if (*action == PlayerUp)
			{
				double thrust;
				if (fuel < 50) {
					std::random_device rd;
					std::mt19937 gen(rd());
					std::uniform_real_distribution<double> dist(-0.1, 0.9);
					thrust = dist(gen);
				}
				else {
					thrust = .5;
					
				}
				if (fuel > 0)
					fuel--;
				else
					thrust = 0;


				if (flip == false) {
					speedY -= thrust;
					
				}
				else {
					speedY += thrust;
				}
			}
			if (*action == PlayerJump) {
				flip = !flip;
				if (getFacing() == Up)
					setFacing(Right);
				else if (getFacing() == Right)
					setFacing(Up);
				else if (getFacing() == Left)
					setFacing(Down);
				else if (getFacing() == Down)
					setFacing(Left);
				else
					cout << "I really need to use case statements\n";
			}
		}

	}
}

bool PlayerEntity::handleCollisions()
{
	for (Entity* entity : collisions)
	{
		grounded = 0;
		if (entity->getType() == Goal) {

			string game = "You Won!\n\n\n\n\n\n\nGame Over";
			DrawText(game.c_str(), 10, 300, 90, BLUE);
			targetX = 1120;
			targetY = 2528;
			//x + 32 * 11, y + 64 * 
			setX(1120);
			setY(2528);
			speedY = 0;
		}
		if (entity->getType() == Obstacle)
		{
			grounded = 2;
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
			if(!grounded)
				if (!flip)
					speedY = -7;
				else
					speedY = 7;
			
		}
		if (entity->getType() == Hurt) {
			targetX = 320;
			targetY = 280;
			setX(320);
			setY(280);
			fuel = 300;
			speedY = 0;
		}
		if (entity->getType() == Fuel) {
			fuel = 300;
		}


	}

	collisions.clear();
	return false;
}
