#include "Obstacle.h"
#include "ModuleFloor.h"
#include <iostream>


Obstacle::Obstacle(SDL_Texture* texture, Enemy* father): Enemy(texture,father)
{
	quad = App->floor->GetQuad(App->floor->lastQuadIndex);
	positionQuad = (float)((RAND()%100)/100.0);
}
Obstacle::~Obstacle() {
}

Enemy* Obstacle::Copy() const {
	Enemy* temp = new Obstacle(texture,father);
	CopyValuesInto(temp);
	return temp;
}
void Obstacle::Update() {
	
	float yScreen = quad->y + quad->h*positionQuad;
	yScreen = (SCREEN_HEIGHT - (yScreen / SCREEN_SIZE));

	position.z = yScreen*CLIPDISTANCE/App->floor->actualCameraY;
	speed.z = lastFrameZ - position.z;
	lastFrameZ = position.z;

	float xOffset = App->player->speedStage;
	position.x -= xOffset;
	
	float scale = 1-(yScreen / App->floor->horizon.y);

	if (shadow) {
		App->shadows->DrawShadow(position.x*scale, yScreen, scale);
	}
	yScreen += position.y*scale;
	int h = (int)(1+anim.GetCurrentFrame().h*scale);
	int w = (int)(1+anim.GetCurrentFrame().w*scale);
	screenPoint = {(int) (position.x*scale),(int)(yScreen),(int)(w),(int)(h) };
	Enemy::Update();
}