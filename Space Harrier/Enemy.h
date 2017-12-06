#ifndef _ENEMY_
#define _ENEMY_

#include "GameObject.h"
#include "Globals.h"
#include "Point.h"
#include "Animation.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleCollision.h"
#include "ModuleRender.h"
#include "ModuleFloor.h"
#include "ModuleShadow.h"
#include "ModuleParticles.h"
#include "ModuleUI.h"


struct SDL_Texture;

class Enemy:public GameObject {
public:
	Enemy(SDL_Texture* texture,Enemy* father=nullptr) :texture(texture),father(father) { 
		if (father != nullptr) {
			father->childs.push_back(this);
		}
	}
	virtual ~Enemy() {}

	virtual Enemy* Copy(const float& x, const float& y, const float& z, Enemy* father = nullptr)const = 0;
	virtual void CopyValuesInto(Enemy& temp, const float& x, const float& y, const float& z)const {
		float screenY = App->floor->GetFloorPositionFromZ(z);

		float scale = 1 - (screenY / App->floor->horizon.y);
		screenY += y*scale;

		int w = anim.GetCurrentFrameConst().w*scale;
		int h = anim.GetCurrentFrameConst().h*scale;

		temp.anim = anim;
		if (noDmg) {
			temp.collider = App->collision->AddCollider({ (int)x,(int)screenY,w,h }, z, speed.z, NO_DMG_ENEMY, &temp);
		}
		else {
			temp.collider = App->collision->AddCollider({ (int)x,(int)screenY,w,h }, z, speed.z, ENEMY, &temp);
		}
		temp.position = {x,y,z};
		temp.speed = speed;
		temp.efxIndex = efxIndex;
		temp.hits = hits;
		temp.noDmg = noDmg;
		temp.destructible = destructible;
		temp.shadow = shadow;
		temp.points = points;
	}
	virtual void Update() {}

	void OnCollision(Collider* other) override {
		if (other->type == PLAYER) {
			return;
		}
		if (destructible) {
			--hits;
			if (hits <= 0) {
				App->ui->AddPoints(points);
				collider->to_delete = true;
				to_delete = true;
				App->particles->AddParticle(App->particles->explosion, position.x, position.y, position.z);
			}
		}
	}

public:
	bool to_delete = false;
	fPoint position = { 0, 0 , 1 };
	Animation anim;
	fPoint speed = {0,0,0};
	unsigned int efxIndex = 0;
	bool firstSound = true;
	int hits=1;
	bool destructible = true;
	bool shadow = true;
	bool noDmg = false;
	int points=0;

	Enemy* father=nullptr;//Only used by enemies with more than 1 collider, they have "parts" which have colliders, and the enemie is part's father
	vector<Enemy*> childs;//Used to delete all components of a part made enemy

	Collider* collider;
	SDL_Texture* texture;
	SDL_Rect screenPoint = {0,0,0,0};

};

#endif // !_ENEMY_

