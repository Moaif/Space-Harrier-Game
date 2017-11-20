#ifndef _ENEMY_
#define _ENEMY_


#include "Globals.h"
#include "Point.h"
#include "Animation.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleCollision.h"
#include "ModuleRender.h"
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

	virtual Enemy* Copy() const { return nullptr; }
	virtual void Update() {}

public:
	bool to_delete = false;
	fPoint position = { 0, 0 , 1 };
	Animation anim;
	float speed;
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

