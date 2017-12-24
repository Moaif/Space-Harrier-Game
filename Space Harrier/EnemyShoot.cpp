#include "EnemyShoot.h"

EnemyShoot::EnemyShoot(SDL_Texture* texture):Particle(texture) {

}

EnemyShoot::~EnemyShoot() {

}

Particle* EnemyShoot::Copy(const float & x, const float & y, const float & z)const {
	Particle* temp = new EnemyShoot(texture);
	float screenY = App->floor->GetFloorPositionFromZ(z);
	float scale = 1 - (screenY / App->floor->horizon.y);
	screenY += y*scale;
	CopyValuesInto(*(temp),x,screenY,z,ENEMY_SHOOT);
	((EnemyShoot*)temp)->initialPos = {x,screenY,z};
	((EnemyShoot*)temp)->pathVector = App->player->GetPlayerCenterPos() - temp->position;
	return temp;
}

void EnemyShoot::Update() {
	if (timeScaled) {
		position.z += -speed * App->time->GetDeltaTime();
	}
	else {
		position.z += -speed * App->time->GetUnscaledDeltaTime();
	}

	float percent =1-( position.z / initialPos.z);
	position.y = initialPos.y + pathVector.y*percent;
	position.x = initialPos.x + pathVector.x*percent;

	float screenY = App->floor->GetFloorPositionFromZ(position.z);

	if (position.z <= 0) {
		collider->to_delete = true;
		to_delete = true;
	}

	float scale = 1 - (screenY / App->floor->horizon.y);

	App->shadows->DrawShadow(position.x, screenY, scale);

	screenPoint.w = 1 + (int)(anim.GetCurrentFrame().w *scale);
	screenPoint.h = 1 + (int)(anim.GetCurrentFrame().h *scale);
	screenPoint.x = (int)position.x;
	screenPoint.y = (int)position.y-screenPoint.h/2;

	Particle::Update();
}

void EnemyShoot::OnCollision(Collider* other){
	timeScaled = true;
}