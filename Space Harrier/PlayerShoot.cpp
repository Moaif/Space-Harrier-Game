#include "PlayerShoot.h"
#include "Enemy.h"
#include "ModuleAudio.h"

PlayerShoot::PlayerShoot(SDL_Texture* texture):Particle(texture) {
}

PlayerShoot::~PlayerShoot() {

}

Particle* PlayerShoot::Copy(const float & x, const float & y, const float & z)const {
	Particle* temp = new PlayerShoot(texture);
	CopyValuesInto(*(temp),x,y,z,LASER);
	((PlayerShoot*)temp)->reboundEfx = reboundEfx;
	return temp;
}

void PlayerShoot::Update() {
	position.z += speed*App->time->GetDeltaTime();
	position.x += reboundXSpeed*App->time->GetDeltaTime();

	if (position.z >= MAX_Z) {
		collider->to_delete = true;
		to_delete = true;
	}

	//shadow manage only
	float screenY = App->floor->GetFloorPositionFromZ(position.z);

	float scale = 1 - (screenY / App->floor->horizon.y);

	if (firstUpdate) {
		reduction = App->player->GetRelativeWorldPosition();
		firstUpdate = false;
	}

	screenPoint.w = 1+(int)(anim.GetCurrentFrame().w*scale);
	screenPoint.h = 1+(int)(anim.GetCurrentFrame().h*scale);
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

void PlayerShoot::OnCollision(Collider* other) {
	if (!((Enemy*)(other->callback))->destructible) {
		App->audio->PlayFx(reboundEfx);
		collider->to_delete = true;
		int sign = RAND() % 2;
		if (sign == 0) {//Positive
			reboundXSpeed = speed * 3;
		}
		else
		{//Negative
			reboundXSpeed = -speed * 3;
		}
		speed = 0;
	}
	else
	{
		collider->to_delete = true;
		to_delete = true;
	}
}