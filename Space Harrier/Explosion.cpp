#include "Explosion.h"

Explosion::Explosion(float fallSpeed,SDL_Texture* texture):Particle(texture),fallSpeed(fallSpeed) {

}

Explosion::~Explosion() {

}

Particle* Explosion::Copy(const float & x, const float & y, const float & z)const{
	Particle* temp = new Explosion(fallSpeed,texture);

	float screenY = App->floor->GetFloorPositionFromZ(z);
	float scale = 1 - (screenY / App->floor->horizon.y);
	float realX = x / scale;

	CopyValuesInto(*(temp), realX, y, z, MAXIMO);
	return temp;
}

void Explosion::Update() {
	position.z -= speed * App->time->GetDeltaTime();
	position.y -= fallSpeed*App->time->GetDeltaTime();
	position.x -= App->player->speedStage*App->time->GetDeltaTime();

	float screenY = App->floor->GetFloorPositionFromZ(position.z);

	float scale = 1 - (screenY / App->floor->horizon.y);

	if (position.y >0) {
		App->shadows->DrawShadow(position.x*scale, screenY, scale);
	}
	screenY += position.y*scale;

	screenPoint.w = 1 + (int)(anim.GetCurrentFrame().w *scale);
	screenPoint.h = 1 + (int)(anim.GetCurrentFrame().h *scale);
	screenPoint.x = (int)(position.x*scale);
	screenPoint.y = (int)(screenY);
}