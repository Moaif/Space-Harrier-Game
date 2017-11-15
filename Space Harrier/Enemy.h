#ifndef _ENEMY_
#define _ENEMY_


#include "Globals.h"
#include "Point.h"
#include "Animation.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleCollision.h"
#include "ModuleRender.h"

struct Collider;

class Enemy {
public:
	Enemy(bool destructible=true):destructible(destructible) {}
	virtual ~Enemy() {}

	virtual Enemy* Copy() const { return nullptr; }
	virtual void Update() {}

public:
	bool to_delete = false;
	fPoint position = { 0, 0 , 1 };
	Animation anim;
	fPoint speed = {0,0,Z_SPEED};
	int hits=1;
	bool destructible = true;

	Collider* collider;
	SDL_Rect screenPoint = {0,0,0,0};

};

#endif // !_ENEMY_

