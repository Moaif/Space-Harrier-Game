#include "Obstacle.h"


Obstacle::Obstacle(bool destructible): Enemy(destructible)
{
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
	position.x -= App->player->speed;
	position.z -= Z_SPEED;
	screenPoint = (App->renderer->ToScreenPoint(position.x, position.y, position.z, &(anim.GetCurrentFrame())));
	collider->rect = screenPoint;
	collider->z = position.z;
	if (position.z <= MIN_Z) {
		collider->to_delete = true;
		to_delete = true;
	}
}