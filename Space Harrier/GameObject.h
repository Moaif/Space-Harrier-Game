#ifndef _GAMEOBJECT_
#define _GAMEOBJECT_

#include "Application.h"
#include "ModuleAnim.h"

struct Collider;
class Animation;

class GameObject {
public:
	~GameObject() {
		App->anim->UnSubscribeAnim(&currentAnimation);
	}
	virtual void OnCollision(Collider* other) {}

public:
	Collider* collider=nullptr;
	Animation* currentAnimation = nullptr;
};

#endif // !_GAMEOBJECT_

