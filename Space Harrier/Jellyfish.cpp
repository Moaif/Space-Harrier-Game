#include "Jellyfish.h"


Jellyfish::Jellyfish(float bounceYMax,SDL_Texture* texture,Enemy* father):Enemy(texture,father),bounceYMax(bounceYMax) {
	quad = App->floor->GetQuad(App->floor->nHorizonQuads-1);
	positionQuad = (float)((RAND() % 100) / 100.0);
}

Jellyfish::~Jellyfish() {

}

Enemy* Jellyfish::Copy(const float& x, const float& y, const float& z) const{
	Enemy* temp = new Jellyfish(bounceYMax,texture, father);
	CopyValuesInto(temp,x,y,z);

	return temp;
}

void Jellyfish::Update() {
	float yScreen = quad->y + quad->h*positionQuad;
	yScreen = (SCREEN_HEIGHT - (yScreen / SCREEN_SIZE));

	position.z = yScreen*CLIPDISTANCE / App->floor->actualCameraY;
	speed.z = lastFrameZ - position.z;
	lastFrameZ = position.z;

	position.x += speed.x * App->time->GetDeltaTime();

	float scale = 1 - (yScreen / App->floor->horizon.y);

	if (shadow) {
		App->shadows->DrawShadow(position.x*scale, yScreen, scale);
	}

	position.y += speed.y*App->time->GetDeltaTime();
	yScreen += (position.y)*scale;
	if (position.y >= bounceYMax || position.y <= 0) {
		speed.y = -speed.y;
	}

	int h = (int)(1 + anim.GetCurrentFrame().h*scale);
	int w = (int)(1 + anim.GetCurrentFrame().w*scale);
	screenPoint = { (int)(position.x*scale),(int)(yScreen),(int)(w),(int)(h) };
	Enemy::Update();

}