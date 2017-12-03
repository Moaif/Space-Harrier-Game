#include "Drone.h"

Drone::Drone(int droneMode,SDL_Texture* texture,Enemy* father):Enemy(texture,father),droneMode(droneMode) {
	elapsedTime = 0.0f;
}

Drone::Drone(int droneMode,fPoint speed, SDL_Texture* texture, Enemy* father) : Enemy(texture, father), 
droneMode(droneMode),tempSpeed(speed) {
	elapsedTime = 0.0f;
}

Drone::~Drone() {
}

Enemy* Drone::Copy(const float& x,const float& y,const float& z, Enemy* father)const {
	Enemy* temp = new Drone(droneMode,speed,texture,father);
	CopyValuesInto(*(temp),x,y,z);
	return temp;
}

void Drone::Update() {
	switch (droneMode)
	{
	case 0:
		Mode0();
		break;
	case 1:
		Mode1();
		break;
	case 2:
		Mode2();
		break;
	case 3:
		Mode3();
		break;

	}
	elapsedTime += App->time->GetDeltaTime();

	float screenY = App->floor->GetFloorPositionFromZ(position.z);

	float scale = 1 - (screenY / App->floor->horizon.y);

	if (shadow) {
		App->shadows->DrawShadow(position.x, screenY, scale);
	}
	screenY += position.y*scale;

	screenPoint.w = 1+(int)(anim.GetCurrentFrame().w *scale);
	screenPoint.h = 1+(int)(anim.GetCurrentFrame().h *scale);
	screenPoint.x = (int)position.x;
	screenPoint.y = (int)(screenY);
	Enemy::Update();
}

void Drone::Shoot() {
	fPoint unitaryVector = App->player->position - position;
	float div = (sqrt((pow(unitaryVector.x, 2) + pow(unitaryVector.y, 2) + pow(unitaryVector.z, 2))));
	unitaryVector = unitaryVector / div;
	App->particles->AddParticle(App->particles->enemyLaser, position.x, position.y + anim.GetCurrentFrame().h / 2, position.z, unitaryVector);
}

void Drone::Mode0() {
	float maxBounce = 100;
	position.y += tempSpeed.y * App->time->GetDeltaTime();
	position.x += tempSpeed.x * App->time->GetDeltaTime();
	position.z += tempSpeed.z * App->time->GetDeltaTime();

	if (position.y >= maxBounce || position.y <= 0) {
		tempSpeed.y = -tempSpeed.y;
	}

	if (elapsedTime < 1.0f) {

	}
	else if (elapsedTime < 2.0f) {
		tempSpeed.x = 0;
		if (!shoted) {
			Shoot();
			shoted = true;
		}
	}
	else{
		if (shoted) {
			Shoot();
			shoted = false;
		}
		tempSpeed.x = -speed.x;
		tempSpeed.y = 0;
	}
}

void Drone::Mode1() {
	position.y += tempSpeed.y * App->time->GetDeltaTime();
	position.x += tempSpeed.x * App->time->GetDeltaTime();
	position.z += tempSpeed.z * App->time->GetDeltaTime();


	if (elapsedTime < 1.0) {
		
	}
	else if (elapsedTime < 2.0f) {
		if (!shoted) {
			Shoot();
			shoted = true;
		}
	}
	else {

		if (shoted) {
			Shoot();
			shoted = false;
		}
		tempSpeed.x = -speed.x;
	}
}

void Drone::Mode2() {
	float maxBounce = 100;
	position.y += tempSpeed.y * App->time->GetDeltaTime();
	position.x += tempSpeed.x * App->time->GetDeltaTime();
	position.z += tempSpeed.z * App->time->GetDeltaTime();

	if (bouncing && (position.y >= maxBounce || position.y <= 0)) {
		tempSpeed.y = -tempSpeed.y;
	}

	if (elapsedTime < 1.0f) {
		tempSpeed.y = 0.0f;
	}
	else if (elapsedTime < 2.0f) {
		if (!bouncing) {
			tempSpeed.y = speed.y;
		}
		bouncing = true;
	}
	else if (elapsedTime < 2.5f) {
		bouncing = false;

		tempSpeed.x = 0;
		tempSpeed.y = speed.y;
	}else{
		if (!shoted) {
			Shoot();
			shoted = true;
		}
		if (position.y >= SCREEN_HEIGHT+anim.GetCurrentFrame().h) {
			collider->to_delete = true;
			to_delete = true;
		}
	}
}

void Drone::Mode3() {
	position.y += tempSpeed.y * App->time->GetDeltaTime();
	position.x += tempSpeed.x * App->time->GetDeltaTime();
	position.z += tempSpeed.z * App->time->GetDeltaTime();


	if (elapsedTime < 1.5f) {

	}
	else if (elapsedTime < 2.5f){
		if (!shoted) {
			Shoot();
			shoted = true;
		}
		tempSpeed.x = -speed.x;
		tempSpeed.y = -speed.y*1.5f;
		tempSpeed.z = speed.z / 2;
	}
	else if(elapsedTime < 2.8f)
	{
		tempSpeed.x = speed.x*2;
		tempSpeed.y = speed.y/3;
		tempSpeed.z = -speed.z;
	}
	else
	{
		if (shoted) {
			Shoot();
			shoted = false;
		}
	}
}
