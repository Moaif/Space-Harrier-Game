#include "Drone.h"
#include "ModuleParticles.h"

Drone::Drone(SDL_Texture* texture,Enemy* father):Enemy(texture,father) {
	quadIndex = 2;
	positionQuad = 1.0f;
	elapsedTime = 0.0f;
}

Drone::~Drone() {
}

Enemy* Drone::Copy(const float& x,const float& y,const float& z)const {
	Enemy* temp = new Drone(texture,father);
	Enemy::CopyValuesInto(temp,x,y,z);
	return temp;
}

void Drone::Update() {
	if (elapsedTime < 2.0f) {
		position.z += speed.z * App->time->GetDeltaTime();
		position.x += speed.x * App->time->GetDeltaTime();
		position.y += speed.y * App->time->GetDeltaTime();
	}
	else if (elapsedTime < 4.0f) {
		position.x -= speed.x * App->time->GetDeltaTime();
		position.y += speed.y * App->time->GetDeltaTime();
		
	}else if(elapsedTime < 6.0f){
		position.z -= speed.z*3 * App->time->GetDeltaTime();
		position.x += speed.x/3 * App->time->GetDeltaTime();
		position.y -= speed.y*2 * App->time->GetDeltaTime();
	}
	elapsedTime += App->time->GetDeltaTime();

	float screenY = App->floor->GetFloorPositionFromZ(position.z);

	float scale = 1 - (screenY / App->floor->horizon.y);

	App->shadows->DrawShadow(position.x, screenY, scale);

	screenY += position.y*scale;

	if (elapsedTime >= 4.0f) {
		if (!shoted) {
			fPoint unitaryVector = App->player->position - fPoint(position.x,position.y,position.z);
			float div = (sqrt((pow(unitaryVector.x, 2) + pow(unitaryVector.y, 2) + pow(unitaryVector.z, 2))));
			unitaryVector.x = unitaryVector.x / div;
			unitaryVector.y = unitaryVector.y / div;
			unitaryVector.z = unitaryVector.z / div;
			App->particles->AddParticle(App->particles->fire, position.x, position.y + anim.GetCurrentFrame().h / 2, position.z, unitaryVector);
			shoted = true;
		}
	}


	screenPoint.w = anim.GetCurrentFrame().w *scale;
	screenPoint.h = anim.GetCurrentFrame().h *scale;

	screenPoint.x = (int)position.x;
	screenPoint.y = (int)(screenY);
	Enemy::Update();
}

/**void Drone::Update() {
	quad = App->floor->GetQuad(quadIndex);
	LOG("I: %d P: %f",quadIndex,positionQuad);
	float yScreen = quad->y + quad->h*positionQuad;
	yScreen = (SCREEN_HEIGHT - (yScreen / SCREEN_SIZE));

	positionQuad -= 2.5*App->time->GetDeltaTime();
	if (positionQuad <= 0) {
		++quadIndex;
		positionQuad = 1.0f;
	}

	position.z = yScreen*CLIPDISTANCE / App->floor->actualCameraY;
	speed.z = lastFrameZ - position.z;
	lastFrameZ = position.z;

	float xOffset = App->player->speedStage;
	position.x -= xOffset;

	float scale = 1 - (yScreen / App->floor->horizon.y);

	if (shadow) {
		App->shadows->DrawShadow(position.x*scale, yScreen, scale);
	}
	yScreen += position.y*scale;
	int h = (int)(1 + anim.GetCurrentFrame().h*scale);
	int w = (int)(1 + anim.GetCurrentFrame().w*scale);
	screenPoint = { (int)(position.x*scale),(int)(yScreen),(int)(w),(int)(h) };
	//Enemy::Update();
}*/