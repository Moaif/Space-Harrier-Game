#include "Obstacle.h"
#include <iostream>


Obstacle::Obstacle(SDL_Texture* texture, Enemy* father): Enemy(texture,father)
{
	quad = App->floor->GetQuad(App->floor->nHorizonQuads-1);
	positionQuad = (float)((RAND()%100)/100.0);
}
Obstacle::~Obstacle() {
}

Enemy* Obstacle::Copy(const float& x, const float& y, const float& z, Enemy* father)const  {
	Enemy* temp = new Obstacle(texture,father);
	CopyValuesInto(*(temp),x,y,z);
	return temp;
}
void Obstacle::Update() {
	
	float yScreen = quad->y + quad->h*positionQuad;
	yScreen = (SCREEN_HEIGHT - (yScreen / SCREEN_SIZE));

	position.z = yScreen*CLIPDISTANCE/App->floor->actualCameraY;
	speed.z = lastFrameZ - position.z;
	lastFrameZ = position.z;

	float xOffset = App->player->speedStage*App->time->GetDeltaTime();
	position.x -= xOffset;
	
	float scale = 1-(yScreen / App->floor->horizon.y);

	if (shadow) {
		App->shadows->DrawShadow(position.x*scale, yScreen, scale);
	}
	yScreen += position.y*scale;
	int w = 1 + (int)(anim.GetCurrentFrame().w *scale);
	int h = 1 + (int)(anim.GetCurrentFrame().h *scale);
	screenPoint = {(int) (position.x*scale),(int)(yScreen),w,h };
	Enemy::Update();
}