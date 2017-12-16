#include "Dragon1.h"
#include "BodyPart.h"

const float Dragon1::MAX_DRAGON_Z = 23.0f;
const float Dragon1::MOVEMENT_UPDATE_TIME = 0.1;
const float Dragon1::MIN_Z_SHOOT = 4.0f;
const float Dragon1::SHOOT_INTERVAL = 0.25;

Dragon1::Dragon1(SDL_Texture* texture, Enemy* father) :Enemy(texture, father) {
	elapsedTime = 0.0f;
	shootTimer = 0.0f;
}

Dragon1::Dragon1(fPoint speed, SDL_Texture* texture, Enemy* father) : Enemy(texture, father){

	partSpeed.push_back(speed);
	for (int i = 1; i < 9; ++i) {//Boss has 9 bodyParts
		partSpeed.push_back({ 0,0,0 });
	}
	elapsedTime = 0.0f;
	shootTimer = 0.0f;
	points = classPoints;
}

Dragon1::~Dragon1() {

}

Enemy* Dragon1::Copy(const float& x, const float& y, const float& z, Enemy* father)const {
	Enemy* temp = new Dragon1(speed, texture, father);
	temp->collider = nullptr;
	temp->speed = speed;
	temp->hits = hits;
	temp->position = { x,y,z };
	int i = 0;
	for (Enemy* e : childs) {
		e->Copy(x, y - e->anim.GetCurrentFrame().h / 2, z, temp);
	}

	return temp;
}

void Dragon1::Update() {

	Movement();
	elapsedTime += App->time->GetUnscaledDeltaTime();

}

void Dragon1::OnCollision(Collider* other) {
	if (other->type == PLAYER) {
		return;
	}
	--hits;
	//Head is especial, we have to push animation twice,other parts just once
	childs[0]->anim.SetNextFrame();
	childs[0]->anim.SetNextFrame();
	for (unsigned int i = 1; i < childs.size(); ++i) {
		childs[i]->anim.SetNextFrame();
	}
	if (hits <= 0) {
		App->ui->AddPoints(points);
		for (unsigned int i = 0; i < childs.size(); ++i) {
			childs[i]->collider->to_delete = true;
			childs[i]->to_delete = true;
			App->particles->AddParticle(*App->particles->explosion, childs[i]->position.x, childs[i]->position.y, childs[i]->position.z);
		}
		to_delete = true;
	}
}

void Dragon1::Shoot() {
	((BodyPart*)childs[0])->Shoot(*App->particles->fire);
}

void Dragon1::Movement() {
	if (childs[0]->position.z < MAX_DRAGON_Z) {
		arrived = true;
	}

	if (elapsedTime > MOVEMENT_UPDATE_TIME) {
		ReorderPartSpeed();
		elapsedTime = 0.0f;

		if (childs[0]->position.y >= SCREEN_HEIGHT + childs[0]->anim.GetCurrentFrame().h) {
			partSpeed[0].y = -speed.y;
		}

		if (childs[0]->position.y <= App->floor->GetFloorPositionFromZ(childs[0]->position.z)) {
			partSpeed[0].y = speed.y;
		}

		if (childs[0]->position.x >= SCREEN_WIDTH / 2) {
			partSpeed[0].x = -speed.x;
		}

		if (childs[0]->position.x <= -(SCREEN_WIDTH / 2)) {
			partSpeed[0].x = speed.x;
		}

		if ((childs[0]->position.z > MAX_DRAGON_Z && arrived)) {
			partSpeed[0].z = speed.z;
			childs[0]->anim.SetPreviousFrame();
		}

		if (childs[0]->position.z < MIN_Z){
			partSpeed[0].z = -speed.z;
			childs[0]->anim.SetNextFrame();
		}

	}

	//Shoot
	if (childs[0]->speed.z < 0 && childs[0]->position.z > MIN_Z_SHOOT) {//If we go to screen
		if (shootTimer >= SHOOT_INTERVAL) {
			Shoot();
			shootTimer = 0.0f;
		}
		else {
			shootTimer += App->time->GetDeltaTime();
		}
	}

	for (int i = 0; i < partSpeed.size(); ++i) {
		childs[i]->speed = partSpeed[i];
		childs[i]->Update();
	}
}

void Dragon1::ReorderPartSpeed() {
	//Set the value of i-1 to i, first one dont get a modification, cause it will be added as new on movement method
	for (int i = partSpeed.size()-1; i > 0; --i) {
		partSpeed[i] = partSpeed[i-1];
	}
}