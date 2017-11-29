#ifndef _PARTICLE_
#define _PARTICLE_

#include "GameObject.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleCollision.h"
#include "ModuleFloor.h"
#include "ModulePlayer.h"
#include "ModuleShadow.h"
#include "ModuleTime.h"

struct SDL_Texture;

class Particle :public GameObject {
public:
	Particle(SDL_Texture* texture):texture(texture){}
	~Particle(){}

	virtual Particle* Copy(const float & x, const float & y, const float & z)const { return nullptr; }
	virtual void CopyValuesInto(Particle& temp,const float & x,const float & y, const float & z,CollisionType colType)const {
		temp.position = { x,y,z };
		temp.efxIndex = efxIndex;
		temp.anim = anim;
		temp.onlyOnce = onlyOnce;
		temp.speed = speed;
		temp.collider = App->collision->AddCollider({(int)x,(int)y,anim.GetCurrentFrameConst().w,anim.GetCurrentFrameConst().h },z, speed, colType, &temp);
	}
	virtual void Update() {
		collider->rect = screenPoint;
		collider->z = position.z;
		collider->speed = speed;
	}

	void OnCollision(Collider* other) override{
		collider->to_delete = true;
		to_delete = true;
		//AddParticle(explosion,(*it)->position.x,(*it)->position.y);
	}

public:
	bool to_delete = false;
	bool firstSound = true;
	bool firstUpdate = true;
	fPoint position = { 0, 0 , 1 };
	unsigned int efxIndex;
	Animation anim;
	bool onlyOnce = false;
	float speed;
	fPoint reduction;

	fPoint pathVector;

	SDL_Texture* texture;
	Collider* collider;
	SDL_Rect screenPoint = { 0,0,0,0 };
};

#endif // !_PARTICLE_

