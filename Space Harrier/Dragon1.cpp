#include "Dragon1.h"
#include "BodyPart.h"

Dragon1::Dragon1(SDL_Texture* texture, Enemy* father):Enemy(texture,father) {
	elapsedTime = 0.0f;
}

Dragon1::Dragon1(const float& startingY,fPoint speed, SDL_Texture* texture, Enemy* father):Enemy(texture,father) {
	for (int i = 0; i < 9; ++i) {//Boss has 9 bodyParts
		partSpeed.push_back(speed);
	}
	elapsedTime = 0.0f;
	points = classPoints;
}

Dragon1::~Dragon1() {

}

Enemy* Dragon1::Copy(const float& x, const float& y, const float& z, Enemy* father)const {
	Enemy* temp = new Dragon1(y,speed,texture,father);
	temp->collider = nullptr;
	temp->speed = speed;
	temp->hits = hits;
	int i = 0;
	for (Enemy* e : childs) {
		e->Copy(x, y, z, temp);
	}
	temp->childs[0]->position.z -= 1;

	fPoint vec = childs[0]->position - childs[(childs.size()-1)]->position;
	vec = vec / 8;// we have 7 body pieces and we dont want the last one over the tail

	for (unsigned int i =1 ; i < (childs.size()-2); ++i) {
		childs[i]->position = position - vec*(i - 1.0f);
	}
}

void Dragon1::Update() {

	if (abs(childs[0]->position.y - startingY) >= 50) {
		partSpeed[0].y = -speed.y;
	}

	Movement();
	elapsedTime += App->time->GetDeltaTime();

}

void Dragon1::OnCollision(Collider* other) {

}

void Dragon1::Shoot() {
	((BodyPart*)childs[0])->Shoot(*App->particles->fire);
}

void Dragon1::Movement() {

}