#include "Dragon3.h"
#include "BodyPart.h"
#include "ModuleEnemy.h"
#include "ModuleScene.h"

const float Dragon3::ARRIVALTIME = 1.0f;
const float Dragon3::TURNAMPLITUDE = 4.0f;
const float Dragon3::BOUNCEMAX = 50.0f;
const float Dragon3::TAILMOVEMENT = 3.0f;
const float Dragon3::TIMEOFFSET = 1.0f / 2.0f;
const float Dragon3::NUMBEROFSHOOTS = 4;

Dragon3::Dragon3(int dragonMode,SDL_Texture* texture,Enemy* father):Enemy(texture,father),dragonMode(dragonMode) {
	elapsedTime = 0.0f;
	timerBetweenShoots = 0;
	shotTimer = 0.0f; 
	actualShot = 0;
}

Dragon3::Dragon3(int dragonMode,float startingY,fPoint speed, SDL_Texture* texture, Enemy* father):Enemy(texture,father),
dragonMode(dragonMode),startingY(startingY) {
	elapsedTime = 0.0f;
	timerBetweenShoots = 0;
	shotTimer = 0.0f;
	actualShot = 0;
	this->headSpeed = { 0,speed.y,speed.z };
	this->tailSpeed = { 0,0,speed.z};
	points = classPoints;
}

Dragon3::~Dragon3() {

}

Enemy* Dragon3::Copy(const float& x, const float& y, const float& z, Enemy* father)const {
	Enemy* temp = new Dragon3(dragonMode,y,speed,texture, father);
	App->scene->numEnemies++;
	temp->collider = nullptr;
	temp->speed = speed;
	temp->hits = hits;
	int i = 0;
	for (Enemy* e:childs) {
		e->Copy(x,y,z,temp);
	}
	temp->childs[0]->position.z -= 1;
	
	//Calculating initial x pos of heads, and its speed to reach final patroll position
	float initialScreenY= App->floor->GetFloorPositionFromZ(MAX_Z);
	float screenY = App->floor->GetFloorPositionFromZ(MAX_Z-speed.z*ARRIVALTIME);
	float initialScale= 1 - (initialScreenY / App->floor->horizon.y);
	float scale = 1 - (screenY / App->floor->horizon.y);
	if (dragonMode == 1) {
		scale *= -1;
	}
	((Dragon3*)temp)->headSpeed.x = ((5 * (childs[2]->anim.GetCurrentFrame().w / 3 * 2)*scale) - (5 * (childs[2]->anim.GetCurrentFrame().w / 3 * 2)*initialScale))/ARRIVALTIME;
	temp->childs[0]->position.x -= 5 * (childs[2]->anim.GetCurrentFrame().w / 3 * 2)*initialScale;
	
	return temp;
}

void Dragon3::Update() {
	childs[0]->speed = headSpeed;
	childs[1]->speed = tailSpeed;
	childs[0]->Update();
	childs[1]->Update();

	if (abs(childs[0]->position.y - startingY) >= 50) {
		headSpeed.y = -headSpeed.y;
	}


	elapsedTime += App->time->GetDeltaTime();

	position = childs[0]->position;

	switch (dragonMode)
	{
	case 0:
		Head1();
		break;
	case 1:
		Head2();
		break;
	}


	//Now i've already moved head and tail, its time to move the body

	fPoint vec = childs[0]->position - childs[1]->position;
	vec = vec / 6;// we have 5 body pieces and we dont want the last one over the tail

	for (int i = 2; i < childs.size(); ++i) {
		childs[i]->position = position - vec*(i - 1);
		childs[i]->Update();
	}


}

void Dragon3::OnCollision(Collider* other) {
	--hits;
	for (int i = 0; i < childs.size(); ++i) {
		childs[i]->anim.SetNextFrame();
	}
	if (hits <= 0) {
		App->ui->AddPoints(points);
		for (int i = 0; i < childs.size(); ++i) {
			childs[i]->collider->to_delete = true;
			childs[i]->to_delete = true;
			App->particles->AddParticle(App->particles->explosion, childs[i]->position.x, childs[i]->position.y, childs[i]->position.z);
		}
		to_delete = true;
		if (--App->scene->numEnemies == 0) {
			App->scene->Win();
		}
	}
}



void Dragon3::Shoot() {
	((BodyPart*)childs[0])->Shoot(App->particles->fire);
}

void Dragon3::Head1(){
	if (elapsedTime < ARRIVALTIME) {

	}
	else if (elapsedTime < ARRIVALTIME+TIMEOFFSET*4) {
		if (timerBetweenShoots >= TIMEOFFSET) {
			if (shotTimer >= TIMEOFFSET / NUMBEROFSHOOTS) {
				Shoot();
				++actualShot;
				if (actualShot >= NUMBEROFSHOOTS) {
					timerBetweenShoots = 0;
					actualShot = 0;
				}
				shotTimer = 0;
			}
			else {
				shotTimer += App->time->GetDeltaTime();
			}
		}
		else {
			timerBetweenShoots += App->time->GetDeltaTime();
		}
		headSpeed.x = 0;
		tailSpeed.x = 0;
	}
	else if (elapsedTime < ARRIVALTIME+(TIMEOFFSET*5)) {
		headSpeed.z = -speed.z;
	}
	else if (elapsedTime < ARRIVALTIME+TIMEOFFSET*6)
	{
		headSpeed.x = -speed.x*TURNAMPLITUDE*2;
		tailSpeed.x = -speed.x*TAILMOVEMENT;
		tailSpeed.z = 0;
	}
	else if (elapsedTime < ARRIVALTIME+TIMEOFFSET*7) {
		headSpeed.x = -speed.x * TURNAMPLITUDE;
		tailSpeed.x = speed.x * TAILMOVEMENT;
		tailSpeed.z = -speed.z;
	}
	else if (elapsedTime < ARRIVALTIME+TIMEOFFSET*11) {
		headSpeed.x = 0;
		tailSpeed.x = 0;
	}
	else if (elapsedTime < ARRIVALTIME+TIMEOFFSET*12) {
		headSpeed.z = speed.z;
	}
	else if (elapsedTime < ARRIVALTIME+TIMEOFFSET*13) {
		headSpeed.x = speed.x * TURNAMPLITUDE*2;
		tailSpeed.x = speed.x * TAILMOVEMENT;
		tailSpeed.z = 0;
	}
	else if (elapsedTime < ARRIVALTIME+TIMEOFFSET*14) {
		headSpeed.x = speed.x * TURNAMPLITUDE;
		tailSpeed.x = -speed.x * TAILMOVEMENT;
		tailSpeed.z = speed.z;
	}
	else
	{
		elapsedTime = ARRIVALTIME;
	}
}

void Dragon3::Head2() {

	if (elapsedTime < ARRIVALTIME) {

	}
	else if (elapsedTime < ARRIVALTIME+TIMEOFFSET*4) {
		if (timerBetweenShoots >= TIMEOFFSET) {
			if (shotTimer >= TIMEOFFSET / NUMBEROFSHOOTS) {
				Shoot();
				++actualShot;
				if (actualShot >= NUMBEROFSHOOTS) {
					timerBetweenShoots = 0;
					actualShot = 0;
				}
				shotTimer = 0;
			}
			else {
				shotTimer += App->time->GetDeltaTime();
			}
		}
		else {
			timerBetweenShoots += App->time->GetDeltaTime();
		}
		headSpeed.z = speed.z;
		headSpeed.x = 0;
		tailSpeed.x = 0;
	}
	else if (elapsedTime < ARRIVALTIME+TIMEOFFSET*5) {
		headSpeed.x = -speed.x * TURNAMPLITUDE;
	}
	else if (elapsedTime < ARRIVALTIME+TIMEOFFSET*6)
	{
		tailSpeed.x = speed.x * TAILMOVEMENT;
		tailSpeed.z = 0;
	}
	else if (elapsedTime < ARRIVALTIME+TIMEOFFSET*7) {
		headSpeed.z = -speed.z * 4;
		tailSpeed.x = -speed.x * TAILMOVEMENT;
		tailSpeed.z = -speed.z;
	}
	else if (elapsedTime < ARRIVALTIME+TIMEOFFSET*11) {
		headSpeed.z = -speed.z;
		headSpeed.x = 0;
		tailSpeed.x = 0;
	}
	else if (elapsedTime < ARRIVALTIME+TIMEOFFSET*12) {
		headSpeed.x = speed.x * TURNAMPLITUDE;
	}
	else if (elapsedTime < ARRIVALTIME+TIMEOFFSET*13) {
		tailSpeed.x = -speed.x * TAILMOVEMENT;
		tailSpeed.z = 0;
	}
	else if (elapsedTime < ARRIVALTIME+TIMEOFFSET*14) {
		headSpeed.z = speed.z * 4;
		tailSpeed.x = speed.x * TAILMOVEMENT;
		tailSpeed.z = speed.z;
	}
	else
	{
		elapsedTime = ARRIVALTIME;
	}
}
