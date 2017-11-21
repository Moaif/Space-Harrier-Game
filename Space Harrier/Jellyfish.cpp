#include "Jellyfish.h"


Jellyfish::Jellyfish(float bounceYMax,SDL_Texture* texture,Enemy* father):Enemy(texture,father),bounceYMax(bounceYMax) {

}

Jellyfish::~Jellyfish() {

}

Enemy* Jellyfish::Copy() const{
	Enemy* temp = new Jellyfish(bounceYMax,texture, father);
	CopyValuesInto(temp);

	return temp;
}

void Jellyfish::Update() {
	position.x += speed.x * App->time->GetDeltaTime();
	position.y += speed.y * App->time->GetDeltaTime();
	position.z -= speed.z * App->time->GetDeltaTime();

	if (position.y >= bounceYMax || position.y <= 0 ) {
			speed.y = -speed.y;
	}
	screenPoint = App->renderer->ToScreenPointBasic(position.x,position.y,position.z,&(anim.GetCurrentFrame()));
	LOG("w: %d h: %d",screenPoint.w,screenPoint.h);

	Enemy::Update();

}