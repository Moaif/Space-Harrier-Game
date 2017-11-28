#include "Fly.h"
#include "ModuleParticles.h"

Fly::Fly(int flyMode, SDL_Texture* texture, Enemy* father):Enemy(texture,father),flyMode(flyMode) {
	elapsedTime = 0.0f;
}

Fly::Fly(int flyMode, fPoint speed, SDL_Texture* texture, Enemy* father): Enemy(texture,father),flyMode(flyMode),tempSpeed(speed) {
	elapsedTime = 0.0f;
}

Fly::~Fly() {
}

Enemy* Fly::Copy(const float& x, const float& y, const float& z, Enemy* father) const {
	Enemy* temp = new Fly(flyMode, speed, texture, father);
	CopyValuesInto(*(temp), x, y, z);
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
	Enemy::Update();
}

void Fly::Shoot() {
	fPoint unitaryVector = App->player->position - position;
	float div = (sqrt((pow(unitaryVector.x, 2) + pow(unitaryVector.y, 2) + pow(unitaryVector.z, 2))));
	unitaryVector = unitaryVector / div;
	App->particles->AddParticle(App->particles->fire, position.x, position.y + anim.GetCurrentFrame().h / 2, position.z, unitaryVector);
}

void Fly::Mode0() {
	position.y += tempSpeed.y * App->time->GetDeltaTime();
	position.x += tempSpeed.x * App->time->GetDeltaTime();
	position.z += tempSpeed.z * App->time->GetDeltaTime();

	if (elapsedTime < 4.5f) {
		if (position.y <= 50) {
			tempSpeed.y = 0;
		}
	}
	else  {
		tempSpeed.x = 0;
		tempSpeed.y = -speed.y*5;
		if (!shoted) {
			Shoot();
			shoted = true;
		}
	}
}

void Fly::Mode1() {
	position.y += tempSpeed.y * App->time->GetDeltaTime();
	position.x += tempSpeed.x * App->time->GetDeltaTime();
	position.z += tempSpeed.z * App->time->GetDeltaTime();

	if (elapsedTime < 4.5f) {
		if (position.y <= 50) {
			tempSpeed.y = 0;
		}
	}
	else {
		tempSpeed.y = -speed.y * 3;
		tempSpeed.x = speed.x * 2;
		tempSpeed.z = speed.z * 1.5f;
		if (!shoted) {
			Shoot();
			shoted = true;
		}
	}
}
