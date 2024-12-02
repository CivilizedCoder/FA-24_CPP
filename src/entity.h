#ifndef ENTITY_H
#define ENTITY_H

#include <vector>

using namespace std;

enum EntityType {Cat, Obstacle, Bounce, Phase, Hurt, Goal, Switch};
enum EntityFacing {Up, Down, Left, Right};

class Entity
{
private:
	float x;
	float y;
	int width;
	int height;
	EntityType type;
	EntityFacing facing;
	bool moving;
	bool falling;

protected:
	vector<Entity*> collisions;

public:
	Entity(float x, float y, int width, int height, EntityType type, EntityFacing facing = Right);

	float getX();
	float getY();
	int getWidth();
	int getHeight();
	EntityType getType();
	EntityFacing getFacing();
	bool getMoving();
	bool getFalling();

	void setX(float x);
	void setY(float y);
	void setFacing(EntityFacing facing);
	void setMoving(bool moving);
	void setFalling(bool falling);

	virtual void tick();

	void addCollision(Entity* entity);
	virtual bool handleCollisions();	
};

#endif
