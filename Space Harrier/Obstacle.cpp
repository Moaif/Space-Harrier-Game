#include "Obstacle.h"


Obstacle::Obstacle(bool destructible, bool noDmg): destructible(destructible), noDmg(noDmg)
{
}
Obstacle::~Obstacle() {
}

Enemy* Obstacle::Copy() const {
	Enemy* temp = new Obstacle(destructible,noDmg);
	temp->anim = anim;
	temp->collider = App->collision->AddCollider(collider->rect,collider->type,collider->callback);
	temp->position = position;
	temp->speed = speed;
	temp->hits = hits;
	
	return temp;
}
void Obstacle::Update() {
	position.x -= App->player->speed;
	position.z -= (speed.z + speed.z * (position.z / MAX_Z)*SCREEN_SIZE);
	screenPoint = (App->renderer->ToScreenPoint(position.x, position.y, position.z, &(anim.GetCurrentFrame())));
	if (position.z <= MIN_Z) {
		to_delete = true;
	}
}