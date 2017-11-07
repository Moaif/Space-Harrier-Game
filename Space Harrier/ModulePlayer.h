#ifndef __ModulePlayer_H__
#define __ModulePlayer_H__

#include "Module.h"
#include "Animation.h"
#include "Point.h"

#define HORIZON_OFFSET 20

struct SDL_Texture;

class ModulePlayer : public Module
{
public:
	ModulePlayer(bool active = true);
	~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(Collider * col,Collider* other);

private:
	void VerifyFlyAnimation();
	void VerifyHorizonX();
	void VerifyHorizonY();

public:

	SDL_Texture* graphics = nullptr;
	Animation* current_animation = nullptr;
	Animation run;
	Animation center;
	Animation left1;
	Animation right1;
	Animation left2;
	Animation right2;
	fPoint position;
	Collider* collider;
	bool destroyed = false;
};

#endif