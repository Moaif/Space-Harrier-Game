#ifndef __ModuleCollision_H__
#define __ModuleCollision_H__

#include<list>
#include "Module.h"

enum CollisionType
{
	PLAYER,
	LASERS,
	WALLS,
	MAXIMO
};

// TODO 9: Create a matrix of game specific types of collision for early discard
// Example: lasers should not collide with lasers but should collider with walls
// enemy shots will collide with other enemies ? and against walls ?

struct Collider
{
	SDL_Rect rect = { 0,0,0,0 };
	CollisionType type;
	Module* callback;
	bool to_delete = false;

	// TODO 10: Add a way to notify other classes that a collision happened

	Collider(SDL_Rect rectangle,CollisionType type,Module* callback) : // expand this call if you need to
		rect(rectangle),type(type),callback(callback)
	{}

	void SetPos(int x, int y)
	{
		rect.x = x;
		rect.y = y;
	}

	bool CheckCollision(const SDL_Rect& r) const;
};

class ModuleCollision : public Module
{
public:

	ModuleCollision();
	~ModuleCollision();

	update_status PreUpdate();
	update_status Update();

	bool CleanUp();

	Collider* AddCollider(const SDL_Rect& rect,CollisionType type,Module* callback);
	void DebugDraw();

private:

	std::list<Collider*> colliders;
	bool debug = false;
	bool hits[MAXIMO][MAXIMO];
};

#endif // __ModuleCollision_H__