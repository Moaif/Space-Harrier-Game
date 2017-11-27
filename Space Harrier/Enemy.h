#ifndef _ENEMY_
#define _ENEMY_


#include "Globals.h"
#include "Point.h"
#include "Animation.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleCollision.h"
#include "ModuleRender.h"
#include "ModuleFloor.h"
#include "ModuleShadow.h"


struct SDL_Texture;

class Enemy {
public:
	Enemy(SDL_Texture* texture,Enemy* father=nullptr) :texture(texture),father(father) { 
		if (father != nullptr) {
			father->childs.push_back(this);
		}
	}
	virtual ~Enemy() {}

	virtual Enemy* Copy(const float& x, const float& y, const float& z,Enemy* father=nullptr) const { return nullptr; }
	virtual void CopyValuesInto(Enemy* temp, const float& x, const float& y, const float& z) const{
		temp->anim = anim;
		temp->collider = App->collision->AddCollider({(int)x,(int)y,collider->rect.w,collider->rect.h}, collider->z, collider->speed, collider->type, collider->callback);
		temp->position = {x,y,z};
		temp->speed = speed;
		temp->hits = hits;
		temp->destructible = destructible;
		temp->shadow = shadow;
	}
	virtual void Update() {
		collider->rect = screenPoint;
		collider->z = position.z;
		collider->speed = speed.z;
		if (position.z <= MIN_Z || position.z >= MAX_Z) {
			collider->to_delete = true;
			to_delete = true;
		}
	}

public:
	bool to_delete = false;
	fPoint position = { 0, 0 , 1 };
	Animation anim;
	fPoint speed = {0,0,0};
	int hits=1;
	bool destructible = true;
	bool shadow = true;

	Enemy* father=nullptr;//Only used by enemies with more than 1 collider, they have "parts" which have colliders, and the enemie is part's father
	list<Enemy*> childs;//Used to delete all components of a part made enemy

	Collider* collider;
	SDL_Texture* texture;
	SDL_Rect screenPoint = {0,0,0,0};

};

#endif // !_ENEMY_

