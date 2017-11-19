#ifndef __ModuleCollision_H__
#define __ModuleCollision_H__

#include<list>
#include "Module.h"

enum CollisionType
{
	PLAYER,
	LASER,
	ENEMY_SHOOT,
	ENEMY,
	NO_DMG_ENEMY,
	MAXIMO
};

struct Collider
{
	SDL_Rect rect = { 0,0,0,0 };
	float z;
	float speed;
	CollisionType type;
	Module* callback;
	bool to_delete = false;


	Collider(SDL_Rect rectangle,float z,float speed,CollisionType type,Module* callback) : 
		rect(rectangle),z(z),speed(speed),type(type),callback(callback)
	{}

	bool CheckCollision(const SDL_Rect& r,float z,float speed) const;
};

class ModuleCollision : public Module
{
public:

	ModuleCollision();
	~ModuleCollision();

	update_status PreUpdate();
	update_status Update();

	bool CleanUp();

	Collider* AddCollider(const SDL_Rect& rect,float z,float speed,CollisionType type,Module* callback);
	void DebugDraw();

private:

	std::list<Collider*> colliders;
	bool debug = false;
	bool hits[MAXIMO][MAXIMO];
};

#endif // __ModuleCollision_H__