#include "PlayerShoot.h"

PlayerShoot::PlayerShoot(SDL_Texture* texture):Particle(texture) {

}

PlayerShoot::~PlayerShoot() {

}

Particle* PlayerShoot::Copy(const float & x, const float & y, const float & z)const {
	Particle* temp = new PlayerShoot(texture);
	CopyValuesInto(*(temp),x,y,z);
	return temp;
}

void PlayerShoot::Update() {
	position.z += speed*App->time->GetDeltaTime();
	if (position.z >= MAX_Z) {
		collider->to_delete = true;
		to_delete = true;
	}

	//shadow manage only
	float screenY = App->floor->GetFloorPositionFromZ(position.z);

	float scale = CLIPDISTANCE / (CLIPDISTANCE + position.z);

	if (firstUpdate) {
		reduction = App->player->GetRelativeWorldPosition();
		firstUpdate = false;
	}

	screenPoint.w = (int)(anim.GetCurrentFrame().w*scale);
	screenPoint.h = (int)(anim.GetCurrentFrame().h*scale);
	screenPoint.x = (int)(position.x - (screenPoint.w*reduction.x*(1 - scale)));

	if (position.y < screenY) {
		screenPoint.y = (int)screenY;
	}
	else
	{
		screenPoint.y = (int)position.y;
		App->shadows->DrawShadow(position.x, screenY, scale);
	}

	Particle::Update();
}