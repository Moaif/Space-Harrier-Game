#include "MetalBall.h"

const float MetalBall::TIMEOFFSET = 1.0f;
const int MetalBall::MAX_ANIM_INDEX_INDESTRUCTIBLE = 4;

MetalBall::MetalBall(int ballMode, SDL_Texture* texture, Enemy* father):Enemy(texture,father),ballMode(ballMode) {
	elapsedTime = 0.0f;
	numberOfShoots = 0;
	destructible = false;
	points = classPoints;
}

MetalBall::MetalBall(int ballMode, fPoint speed,float animSpeed, SDL_Texture* texture, Enemy* father):Enemy(texture,father),ballMode(ballMode),
tempSpeed(speed),initialAnimSpeed(animSpeed){
	elapsedTime = 0.0f;
	numberOfShoots = 0;
}

MetalBall::~MetalBall() {

}

Enemy* MetalBall::Copy(const float& x, const float& y, const float& z, Enemy* father)const {
	Enemy* temp = new MetalBall(ballMode,speed,anim.speed,texture,father);
	CopyValuesInto(*temp,x,y,z);
	return temp;
}

void MetalBall::Update() {
	switch (ballMode)
	{
	case 0:
		Mode1();
		break;

	}
	elapsedTime += App->time->GetDeltaTime();

	//Verification of actual sprite to set it indestructible or not
	if (anim.GetCurrentFrameIndex() < MAX_ANIM_INDEX_INDESTRUCTIBLE) {
		destructible = false;
	}
	else {
		destructible = true;
	}

	float screenY = App->floor->GetFloorPositionFromZ(position.z);

	float scale = 1 - (screenY / App->floor->horizon.y);

	if (shadow) {
		App->shadows->DrawShadow(position.x, screenY, scale);
	}
	screenY += position.y*scale;

	screenPoint.w = 1 + (int)(anim.GetCurrentFrame().w *scale);
	screenPoint.h = 1 + (int)(anim.GetCurrentFrame().h *scale);
	screenPoint.x = (int)position.x;
	screenPoint.y = (int)(screenY);

	collider->rect = screenPoint;
	collider->z = position.z;
	collider->speed = tempSpeed.z;
	if (position.z <= MIN_Z || position.z >= MAX_Z) {
		collider->to_delete = true;
		to_delete = true;
	}
}

void MetalBall::Shoot() {
	App->particles->AddParticle(*App->particles->fire,position.x,position.y+anim.GetCurrentFrame().h/2,position.z);
	++numberOfShoots;
}

void MetalBall::Mode1() {
	if (elapsedTime < TIMEOFFSET*0.5f) {
		anim.speed = 0;
		tempSpeed = { 0,0,speed.z };
	}
	else if (elapsedTime < TIMEOFFSET*1.0f) {
		tempSpeed = { speed.x,speed.y,0};
	}
	else if(numberOfShoots < 3){
		anim.speed = initialAnimSpeed;
		if (first) {
			tempSpeed = { 0,0,0 };
			first = false;
		}
		//Finished Opening
		if (anim.Finished() && !anim.inversed) {
			anim.inversed = true;
			anim.Reset();
			Shoot();
			tempSpeed = { 0,0,0 };
		}
		//Finished Closing
		else if (anim.Finished() && anim.inversed) {
			anim.inversed = false;
			anim.Reset();
			tempSpeed = { 0,-abs(speed.y/5),speed.z };
		}
		elapsedTime = 1.0f;
	}
	else if (elapsedTime < 1.5f) {
		anim.inversed = false;
		anim.Reset();
		anim.speed = 0;
		tempSpeed = { -speed.x,-speed.y,0 };
	}
	else {
		tempSpeed = { 0,0,-speed.z };
	}

	position += tempSpeed * App->time->GetDeltaTime();
}