#include "Obstacle.h"
#include "ModuleFloor.h"
#include <iostream>


Obstacle::Obstacle(bool destructible): Enemy(destructible)
{
	quad = App->floor->GetQuad(App->floor->lastQuadIndex);
	positionQuad = (RAND()%100)/100.0;
	position.z = MAX_Z;
}
Obstacle::~Obstacle() {
}

Enemy* Obstacle::Copy() const {
	Enemy* temp = new Obstacle(destructible);
	temp->anim = anim;
	temp->collider = App->collision->AddCollider(collider->rect,collider->z,collider->type,collider->callback);
	temp->position = position;
	temp->speed = speed;
	temp->hits = hits;
	
	return temp;
}
void Obstacle::Update() {
	
	float yScreen = quad->y + quad->h*positionQuad;
	yScreen = (SCREEN_HEIGHT - (yScreen / SCREEN_SIZE));
	yScreen += position.y;
	//position.z = yScreen*3/3;

	float xOffset = App->player->speed;
	position.x -= xOffset;
	
	float scale = 1-(yScreen / App->floor->horizon.y);
	int h = 1+anim.GetCurrentFrame().h*scale;
	int w = 1+anim.GetCurrentFrame().w*scale;
	screenPoint = {(int) (position.x*scale),(int)(yScreen),(int)(w),(int)(h) };

	collider->rect = screenPoint;
	collider->z = position.z;
	if (position.z <= MIN_Z) {
		collider->to_delete = true;
		to_delete = true;
	}
}