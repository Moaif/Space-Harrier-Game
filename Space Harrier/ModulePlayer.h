#ifndef __ModulePlayer_H__
#define __ModulePlayer_H__

#include "Module.h"
#include "Animation.h"
#include "Point.h"
#include "GameObject.h"

struct SDL_Texture;

class ModulePlayer : public Module,public GameObject
{
public:
	ModulePlayer(bool active = true);
	~ModulePlayer();

	bool Start();
	bool Restart();
	update_status Update();
	bool CleanUp();
	void OnCollision(Collider* other)override;

	void SetLives(int amount);
	int GetLives();
	fPoint GetRelativeWorldPosition();
	void Win();

private:
	void VerifyFlyAnimation();
	void VerifyFloorSpeed();
	void VerifyHorizonX();
	void VerifyHorizonY();
	void CalculateSpeed();
	void Death();
	void AnimWin();

public:

	SDL_Texture* graphics = nullptr;
	Animation* current_animation = nullptr;
	Animation run;
	Animation center;
	Animation left1;
	Animation right1;
	Animation left2;
	Animation right2;
	Animation fall;
	Animation death;
	fPoint position;
	Collider* collider;
	bool destroyed = false;
	float speedStage=0.0f;

private:
	int lives = 0;
	//Horizon manage
	float actualHorizonPercentage;
	//Death anim
	bool hit = false;
	bool deathBounced = false;
	float deathStartingPosY=0.0f;
	//Death recover
	float recoverTimer = 0.0f;
	//Audio
	unsigned int ouch = 0;
	unsigned int aaargh = 0;
	unsigned int getReady = 0;
	//Win anim variables
	bool win = false;
	bool centered=false;
	fPoint initialPosition;
	float currentLerpPercentaje;
	static const int INITIAL_LIVES;
	static const float MIN_X_SPEED;
	static const float MAX_X_SPEED;
	static const float HORIZON_Y_SPEED;
	static const float MOVEMENT_SPEED;
	static const float SCREEN_SEGMENT;
	static const float FALL_SPEED;
	static const float FALL_BOUNCE;
	static const float RECOVER_TIME;
	static const float COLLIDER_W;
	static const float COLLIDER_H;
};

#endif