#include "Fly.h"

const float Fly::TIMEOFFSET = 1.0f / 2.0f;

Fly::Fly(int flyMode, SDL_Texture* texture, Enemy* father):Enemy(texture,father),flyMode(flyMode) {
	elapsedTime = 0.0f;
	points = classPoints;
}

Fly::Fly(int flyMode, fPoint speed, SDL_Texture* texture, Enemy* father): Enemy(texture,father),flyMode(flyMode),tempSpeed(speed) {
	elapsedTime = 0.0f;
}

Fly::~Fly() {
}

Enemy* Fly::Copy(const float& x, const float& y, const float& z, Enemy* father) const {
	Enemy* temp = new Fly(flyMode, speed, texture, father);
	CopyValuesInto(*temp, x, y, z);
	return temp;
}

void Fly::Update() {
	switch (flyMode)
	{
	case 0:
		Mode0();
		break;
	case 1:
		Mode1();
		break;

	}
	elapsedTime += App->time->GetDeltaTime();

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

void Fly::Shoot() {
	App->particles->AddParticle(*App->particles->fire, position.x, position.y + anim.GetCurrentFrame().h / 2, position.z);
}

void Fly::Mode0() {
	position += tempSpeed * App->time->GetDeltaTime();

	if (elapsedTime < TIMEOFFSET*4.5f) {
		if (position.y <= 50) {
			tempSpeed.y = 0;
		}
	}
	else  {
		tempSpeed = { 0,-speed.y * 5,speed.z };
		if (!shoted) {
			Shoot();
			shoted = true;
		}
	}
}

void Fly::Mode1() {
	position += tempSpeed * App->time->GetDeltaTime();

	if (elapsedTime < TIMEOFFSET*4.5f) {
		if (position.y <= 50) {
			tempSpeed.y = 0;
		}
	}
	else {
		tempSpeed = { speed.x * 2,-speed.y * 3 ,speed.z * 1.5f };
		if (!shoted) {
			Shoot();
			shoted = true;
		}
	}
}
