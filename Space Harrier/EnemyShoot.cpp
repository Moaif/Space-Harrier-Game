#include "EnemyShoot.h"

EnemyShoot::EnemyShoot(SDL_Texture* texture):Particle(texture) {

}

EnemyShoot::~EnemyShoot() {

}

Particle* EnemyShoot::Copy(const float & x, const float & y, const float & z)const {
	Particle* temp = new EnemyShoot(texture);
	CopyValuesInto(*(temp),x,y,z);
	return temp;
}

void EnemyShoot::Update() {
	position.x += speed * pathVector.x * App->time->GetDeltaTime();
	position.y += speed * pathVector.y * App->time->GetDeltaTime();
	position.z += speed * pathVector.z * App->time->GetDeltaTime();

	float screenY = App->floor->GetFloorPositionFromZ(position.z);

	if (position.z <= 0) {
		collider->to_delete = true;
		to_delete = true;
	}

	float scale = 1 - (screenY / App->floor->horizon.y);

	App->shadows->DrawShadow(position.x, screenY, scale);

	screenY += position.y*scale;

	screenPoint.w = 1 + (int)(anim.GetCurrentFrame().w *scale);
	screenPoint.h = 1 + (int)(anim.GetCurrentFrame().h *scale);
	screenPoint.x = (int)position.x;
	screenPoint.y = (int)screenY;

	Particle::Update();
}