#include <map>
#include <string>
#include "entity_view.h"
#include "textures.h"

struct SpriteSheetInfo
{
	string fileName;
	int numFrames = 0;
	map<EntityFacing, int> facings;
};

map<EntityType, SpriteSheetInfo> spriteMapping = {
	{Cat, {"resources/catsprites.png", 3, {
		{Right, 0}, 
		{Up, 1}, 
		{Down, 2}, 
		{Left, 3}}
	}},
	{Obstacle, {"resources/gray_wall.png", 1, {
		{Right, 0}}
	}}
,
{ Bounce, {"resources/wall.png", 1, {
	{Right, 0}}
} }
};

EntityView::EntityView(Entity *entity)
{
	this->entity = entity;
	texture = Textures::loadTexture(spriteMapping[entity->getType()].fileName);
	frame = 0;
	delay = 0;
}

bool EntityView::isViewFor(Entity* entity)
{
	return this->entity == entity;
}

void EntityView::draw(int vx, int vy, int vdx, int vdy, int vw, int vh)
{

	int animationDelay = 20;
	int numFrames = spriteMapping[entity->getType()].numFrames;

	if (entity->getMoving())
	{
		delay--;

		if (delay == 0)
		{
			frame = (frame + 1) % numFrames;
			delay = animationDelay;
		}
	}
	else
	{
		frame = 0;
		delay = animationDelay;
	}


	int frameWidth = texture.width / numFrames;
	int row = spriteMapping[entity->getType()].facings[entity->getFacing()];
	Rectangle frameRec = { frame * frameWidth, row * frameWidth, frameWidth, frameWidth };

	float x = (entity->getX() - vx) + vdx;
	float y = (entity->getY() - vy) + vdy;

	DrawTextureRec(texture, frameRec, { x, y}, WHITE);
}