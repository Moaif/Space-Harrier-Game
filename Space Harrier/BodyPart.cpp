#include "BodyPart.h"

BodyPart::BodyPart(SDL_Texture* texture,Enemy* father):Enemy(texture,father) {

}

BodyPart::~BodyPart() {

}

Enemy* BodyPart::Copy(const float& x, const float& y, const float& z, Enemy* father)const {
	Enemy* temp = new BodyPart(texture, father);
	CopyValuesInto(*(temp), x, y, z);
	return temp;
}

void BodyPart::Update() {
	position += speed * App->time->GetUnscaledDeltaTime();

	float screenY = App->floor->GetFloorPositionFromZ(position.z);

	float scale = 1 - (screenY / App->floor->horizon.y);

	if (shadow) {
		App->shadows->DrawShadow(position.x, screenY, scale);
	}
	screenY += position.y*scale;

	screenPoint.w = 1 + (int)(anim.GetCurrentFrame().w *scale);
	screenPoint.h = 1 + (int)(anim.GetCurrentFrame().h *scale);
	screenPoint.x = (int)position.x;
	screenPoint.y = (int)(screenY);

	collider->rect = screenPoint;
	collider->z = position.z;
	collider->speed = speed.z;
}

void BodyPart::OnCollision(Collider* other) {
	if (destructible) {
		father->OnCollision(other);
	}
}


void BodyPart::Shoot(const Particle& p) {
	App->particles->AddParticle(p, position.x, position.y + anim.GetCurrentFrame().h / 2, position.z);
}