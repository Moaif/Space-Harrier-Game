#include "Obstacle.h"
#include <iostream>


Obstacle::Obstacle(SDL_Texture* texture, Enemy* father): Enemy(texture,father)
{
	quad = App->floor->GetQuad(App->floor->nHorizonQuads-1);
	positionQuad = (float)((RAND()%100)/100.0);
	points = classPoints;
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
	if (App->time->GetDeltaTime() > 0.0f) {//If not checked, after a pause, it may be 0, and speed would turn into infinite/nan
		speed.z = (lastFrameZ - position.z) / App->time->GetDeltaTime();
	}
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

	collider->rect = screenPoint;
	collider->z = position.z;
	collider->speed = speed.z;

	//Verify that this object wont return and made more than 1 loop from horizon to screen
	if (App->floor->nextFrameItChange) {
		++counter;
		if (counter == App->floor->nHorizonQuads - 1) {
			collider->to_delete = true;
			to_delete = true;
		}
	}

}

void Obstacle::OnCollision(Collider* other) {
	if (other->type == PLAYER) {
		return;
	}
	if (destructible) {
		--hits;
		if (hits <= 0) {
			App->ui->AddPoints(points);
			collider->to_delete = true;
			to_delete = true;

			float yScreen = quad->y + quad->h*positionQuad;
			yScreen = (SCREEN_HEIGHT - (yScreen / SCREEN_SIZE));
			float scale = 1 - (yScreen / App->floor->horizon.y);
			App->particles->AddParticle(*App->particles->explosion, position.x*scale, position.y, position.z);
		}
	}
}