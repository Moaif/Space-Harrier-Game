#include "BodyPart.h"
#include "ModuleParticles.h"

BodyPart::BodyPart(SDL_Texture* texture,Enemy* father):Enemy(texture,father) {

}

BodyPart::~BodyPart() {

}

Enemy* BodyPart::Copy(const float& x, const float& y, const float& z,Enemy* father) const {
	Enemy* temp = new BodyPart(texture, father);
	CopyValuesInto(*(temp), x, y, z);
	return temp;
}

void BodyPart::Update() {
	position.y += speed.y * App->time->GetDeltaTime();
	position.x += speed.x * App->time->GetDeltaTime();
	position.z += speed.z * App->time->GetDeltaTime();

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
		--(father->hits);
		if (father->hits <= 0) {
			for (list<Enemy*>::iterator cIt = father->childs.begin(); cIt != father->childs.end(); ++cIt) {
				(*cIt)->collider->to_delete = true;
				(*cIt)->to_delete = true;//TODO hacer OnDestroy() para enemies
			}
			father->to_delete = true;
		}
	}
}


void BodyPart::Shoot(Particle* p) {
	fPoint unitaryVector = App->player->position - position;
	float div = (sqrt((pow(unitaryVector.x, 2) + pow(unitaryVector.y, 2) + pow(unitaryVector.z, 2))));
	unitaryVector = unitaryVector / div;
	App->particles->AddParticle(p, position.x, position.y + anim.GetCurrentFrame().h / 2, position.z, unitaryVector);
}