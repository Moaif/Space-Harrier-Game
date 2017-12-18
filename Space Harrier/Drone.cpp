#include "Drone.h"

const float Drone::TIMEOFFSET = 1.0f*2;

Drone::Drone(int droneMode,SDL_Texture* texture,Enemy* father):Enemy(texture,father),droneMode(droneMode) {
	elapsedTime = 0.0f;
	points = classPoints;
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
		Mode3_1();
		break;
	case 1:
		Mode3_2(true);
		break;
	case 2:
		Mode3_3();
		break;
	case 3:
		Mode3_4();
		break;
	case 4:
		Mode1_1();
		break;
	case 5:
		Mode1_2();
		break;
	case 6:
		Mode3_2(false);//Same movement, but only 1 shoot
		break;
	case 7:
		Mode1_3();
		break;
	case 8:
		Mode1_4();
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

	collider->rect = screenPoint;
	collider->z = position.z;
	collider->speed = tempSpeed.z;
	if (position.z <= MIN_Z || position.z >= MAX_Z) {
		collider->to_delete = true;
		to_delete = true;
	}
}

void Drone::Shoot() {
	App->particles->AddParticle(*App->particles->enemyLaser, position.x, position.y + anim.GetCurrentFrame().h / 2, position.z);
}

void Drone::Mode3_1() {
	float maxBounce = 100;
	position += tempSpeed * App->time->GetDeltaTime();

	if (position.y >= maxBounce || position.y <= 0) {
		tempSpeed.y = -tempSpeed.y;
	}

	if (elapsedTime < TIMEOFFSET) {

	}
	else if (elapsedTime < TIMEOFFSET*2) {
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
		tempSpeed = { -speed.x ,0,speed.z };
	}
}

void Drone::Mode3_2(bool doubleShoot) {
	position += tempSpeed * App->time->GetDeltaTime();


	if (elapsedTime < TIMEOFFSET) {
		
	}
	else if (elapsedTime < TIMEOFFSET*2) {
		if (!shoted) {
			Shoot();
			shoted = true;
		}
	}
	else {
		if (doubleShoot) {
			if (shoted) {
				Shoot();
				shoted = false;
			}
		}
		tempSpeed.x = -speed.x;
	}
}

void Drone::Mode3_3() {
	float maxBounce = 100;
	position += tempSpeed * App->time->GetDeltaTime();

	if (bouncing && (position.y >= maxBounce || position.y <= 0)) {
		tempSpeed.y = -tempSpeed.y;
	}

	if (elapsedTime < TIMEOFFSET*0.5) {
		tempSpeed.y = 0.0f;
	}
	else if (elapsedTime < TIMEOFFSET) {
		if (!bouncing) {
			tempSpeed.y = speed.y;
		}
		bouncing = true;
	}
	else if (elapsedTime < TIMEOFFSET*1.5f) {
		bouncing = false;
		tempSpeed = { 0,speed.y,speed.z };
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

void Drone::Mode3_4() {
	position += tempSpeed * App->time->GetDeltaTime();


	if (elapsedTime < TIMEOFFSET*1.5f) {

	}
	else if (elapsedTime < TIMEOFFSET*2.5f){
		if (!shoted) {
			Shoot();
			shoted = true;
		}
		tempSpeed = { -speed.x ,-speed.y*1.5f ,speed.z / 2 };
	}
	else if(elapsedTime < TIMEOFFSET*2.8f)
	{
		tempSpeed = { speed.x * 2 ,speed.y / 3,-speed.z };
	}
	else
	{
		if (shoted) {
			Shoot();
			shoted = false;
		}
	}
}

void Drone::Mode1_1() {
	position += tempSpeed * App->time->GetDeltaTime();

	if (position.y <= 0) {
		position.y = 0;
	}

	if (elapsedTime < TIMEOFFSET*1.8f) {
		tempSpeed = {speed.x,0,0};
	}
	else{
		tempSpeed = { -speed.x * 2 ,speed.y, speed.z };
	}
}

void Drone::Mode1_2() {
	position += tempSpeed * App->time->GetDeltaTime();

	if (position.y <= 0) {
		position.y = 0;
	}

	if (elapsedTime < TIMEOFFSET*1.0f) {

	}
	else if(elapsedTime < TIMEOFFSET*2.0f){
		tempSpeed = { 0,-speed.y * 3 , 0};
	}else if(elapsedTime < TIMEOFFSET*3.0f){
		if (!shoted) {
			Shoot();
			shoted = true;
		}
		tempSpeed.y = speed.y * 3;
	}
	else {
		tempSpeed = { speed.x / 4,-speed.y / 4,speed.z * 2 };
	}
}

void Drone::Mode1_3() {
	position += tempSpeed * App->time->GetDeltaTime();

	if (elapsedTime < TIMEOFFSET*1.0f) {

	}
	else if (elapsedTime < TIMEOFFSET*1.5f) {
		tempSpeed = { -speed.x / 2,speed.y * 4,speed.z / 2 };
	}
	else if (elapsedTime < TIMEOFFSET*2.0f) {
		tempSpeed = { -speed.x,-speed.y * 4 ,-speed.z / 2 };
	}
	else
	{
		tempSpeed = { speed.x / 2.0f,-speed.y,-speed.z / 1.5f };
	}
}

void Drone::Mode1_4() {
	position += tempSpeed * App->time->GetDeltaTime();

	if (elapsedTime < TIMEOFFSET*1.0f) {

	}
	else if (elapsedTime < TIMEOFFSET*1.25f) {
		tempSpeed.x = 0;
	}
	else if (elapsedTime < TIMEOFFSET*1.65f) {
		tempSpeed = { -speed.x / 1.3f ,speed.z / 5.0f ,0.0f};
	}
	else if (elapsedTime < TIMEOFFSET*2.0f) {
		tempSpeed = { -speed.x / 1.3f ,0.0f,-speed.z / 5.0f };
	}
	else {
		if (!shoted) {
			Shoot();
			shoted = true;
		}
		tempSpeed = { -speed.x / 4 ,-speed.y,-speed.z };
	}
}