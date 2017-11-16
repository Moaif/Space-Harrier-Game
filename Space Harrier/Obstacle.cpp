#include "Obstacle.h"


Obstacle::Obstacle(bool destructible): Enemy(destructible)
{
	quad = App->renderer->GetQuad(10);
	positionQuad = 0.0f;
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
	position.z = MIN_Z +((( (SCREEN_HEIGHT-(quad->y/SCREEN_SIZE))+quad->h*positionQuad)/App->renderer->horizon.y)*(MAX_Z -MIN_Z));
	LOG("Z: %f quadY: %d quadH: %d hY: %d",position.z, ((SCREEN_HEIGHT - (quad->y / SCREEN_SIZE))),quad->h,App->renderer->horizon.y);
	screenPoint = (App->renderer->ToScreenPoint(position.x, position.y, position.z, &(anim.GetCurrentFrame())));
	collider->rect = screenPoint;
	collider->z = position.z;
	if (position.z <= MIN_Z) {
		collider->to_delete = true;
		to_delete = true;
	}
}