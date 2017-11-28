#ifndef _GAMEOBJECT_
#define _GAMEOBJECT_

struct Collider;

class GameObject {
public:
	virtual void OnCollision(Collider* other) {}

public:
	Collider* collider;
};

#endif // !_GAMEOBJECT_

