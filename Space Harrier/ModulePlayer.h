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

	bool Start()override;
	bool Restart()override;
	update_status Update()override;
	bool CleanUp()override;
	void OnCollision(Collider* other)override;

	void SetLives(const int& amount);
	int GetLives()const;
	fPoint GetRelativeWorldPosition() const;
	fPoint GetPlayerCenterPos()const;
	void Win();

private:
	void VerifyFlyAnimation();
	void VerifyFloorSpeed();
	void VerifyHorizonX();
	void VerifyHorizonY();
	void CalculateSpeed();
	void Death();
	void AnimWin();
	void AnimWelcome();

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
	//Welcome anim
	bool welcome = false;
	float welcomeTimer;
	//Shoot
	float shootTimer;
	//Static variables
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
	static const float WELCOME_RUN_TIME;
	static const float WELCOME_TRANSITION_TIME;
	static const float SHOOT_INTERVAL;
};

#endif