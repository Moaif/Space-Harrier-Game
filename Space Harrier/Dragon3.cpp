#include "Dragon3.h"
#include "BodyPart.h"
#include "ModuleParticles.h"
#include "ModuleEnemy.h"

Dragon3::Dragon3(SDL_Texture* texture,Enemy* father):Enemy(texture,father) {

}

Dragon3::~Dragon3() {

}

Enemy* Dragon3::Copy(const float& x, const float& y, const float& z, Enemy* father)const {
	Enemy* temp = new Dragon3(texture, father);
	temp->collider = nullptr;
	temp->speed = speed;
	temp->hits = hits;
	int i = 0;
	for (Enemy* e:childs) {
		e->Copy(x+(i*10),y,z-(i/5),temp);
		if (destructible) {
			--i;
		}
		else
		{
			++i;
		}
	}
	return temp;
}

void Dragon3::Update() {
	list<Enemy*>::iterator it = childs.begin();
	for (int i = 0; i < 2; ++i) {
		(*it)->speed = speed;
		(*it)->Update();
		++it;
	}
	
	//Now i've already moved head and tail, its time to move the body

	while (it != childs.end()) {
		(*it)->Update();
		++it;
	}

}

void Dragon3::Shoot() {
	((BodyPart*)childs.front())->Shoot(App->particles->fire);
}
