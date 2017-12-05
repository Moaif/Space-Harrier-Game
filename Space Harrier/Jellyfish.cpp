#include "Jellyfish.h"


Jellyfish::Jellyfish(float bounceYMax,SDL_Texture* texture,Enemy* father):Enemy(texture,father),bounceYMax(bounceYMax) {
	quad = App->floor->GetQuad(App->floor->nHorizonQuads-1);
	positionQuad = (float)((RAND() % 100) / 100.0);
	points = classPoints;
}

Jellyfish::~Jellyfish() {

}

Enemy* Jellyfish::Copy(const float& x, const float& y, const float& z, Enemy* father) const{
	Enemy* temp = new Jellyfish(bounceYMax,texture, father);
	CopyValuesInto(*(temp),x,y,z);
	return temp;
}

void Jellyfish::Update() {
	float yScreen = quad->y + quad->h*positionQuad;
	yScreen = (SCREEN_HEIGHT - (yScreen / SCREEN_SIZE));

	position.z = yScreen*CLIPDISTANCE / App->floor->actualCameraY;
	speed.z = (lastFrameZ - position.z) / App->time->GetDeltaTime();
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

	int w = 1 + (int)(anim.GetCurrentFrame().w *scale);
	int h = 1 + (int)(anim.GetCurrentFrame().h *scale);
	screenPoint = { (int)(position.x*scale),(int)(yScreen),w,h };
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

void Jellyfish::OnCollision(Collider* other) {
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
			App->particles->AddParticle(App->particles->explosion, position.x*scale, position.y, position.z);
		}
	}
}