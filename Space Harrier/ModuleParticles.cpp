#include <math.h>
#include "ModuleParticles.h"
#include "Application.h"
#include "ModuleAudio.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"
#include "ModuleTime.h"
#include "ModulePlayer.h"
#include "ModuleFloor.h"
#include "ModuleShadow.h"

#include "SDL/include/SDL_timer.h"

ModuleParticles::ModuleParticles()
{}

ModuleParticles::~ModuleParticles()
{}

// Load assets
bool ModuleParticles::Start()
{
	LOG("Loading particles");
	lasers = App->textures->Load("assets/Shoots.png");
	shots = App->textures->Load("assets/Shoots2.png");


	// Creating shoot particle
	laser.anim.frames.push_back({ 0, 0, 56, 38 });
	laser.anim.frames.push_back({ 57, 0, 56, 38 });
	laser.anim.frames.push_back({ 113, 0, 56, 38 });
	laser.anim.frames.push_back({ 171, 0, 56, 38 });
	laser.anim.randFrame = true;
	laser.anim.speed = 1.0f;
	laser.efxIndex = App->audio->LoadFx("assets/laser.wav");
	laser.speed = 40.0f;
	laser.collider = new Collider({ 0,0,91,61 },1,laser.speed,LASER,this);
	laser.texture = lasers;

	fire.anim.frames.push_back({1,1,51,47});
	fire.anim.frames.push_back({ 56,2,48,44 });
	fire.anim.frames.push_back({ 110,0,50,48 });
	fire.anim.speed = 5.0f;
	fire.speed = 40.0f;
	fire.collider = new Collider({0,0,51,48},1,fire.speed,ENEMY_SHOOT,this);
	fire.texture = shots;


	explosion.anim.frames.push_back({ 274, 296, 33, 30 });
	explosion.anim.frames.push_back({ 313, 296, 33, 30 });
	explosion.anim.frames.push_back({ 346, 296, 33, 30 });
	explosion.anim.frames.push_back({ 382, 296, 33, 30 });
	explosion.anim.frames.push_back({ 419, 296, 33, 30 });
	explosion.anim.frames.push_back({ 457, 296, 33, 30 });
	explosion.efxIndex = App->audio->LoadFx("assets/explosion.wav");
	explosion.speed = 0;
	explosion.collider = nullptr;
	explosion.onlyOnce = true;

	return true;
}

// Unload assets
bool ModuleParticles::CleanUp()
{
	LOG("Unloading particles");
	App->textures->Unload(lasers);

	for (list<Particle*>::iterator it = active.begin(); it != active.end(); ++it)
		RELEASE(*it);

	active.clear();

	return true;
}

// PreUpdate to clear up all dirty particles
update_status ModuleParticles::PreUpdate()
{
	for (list<Particle*>::iterator it = active.begin(); it != active.end();)
	{
		if ((*it)->to_delete == true)
		{
			RELEASE(*it);
			it = active.erase(it);
		}
		else
			++it;
	}

	return UPDATE_CONTINUE;
}

// Update all particle logic and draw them
update_status ModuleParticles::Update()
{
	for (list<Particle*>::iterator it = active.begin(); it != active.end(); ++it)
	{
		Particle* p = *it;

		p->Update();
		if (p->screenPoint.h == 0 && p->screenPoint.w == 0) {
			App->renderer->AddToBlitBuffer(p->texture, p->position.x, p->position.y, p->position.z, &(p->anim.GetCurrentFrame()), nullptr);
		}
		else
		{
			resizeStruct resizeInfo = { p->screenPoint.w,p->screenPoint.h };
			App->renderer->AddToBlitBuffer(p->texture, (float)p->screenPoint.x, (float)p->screenPoint.y, p->position.z, &(p->anim.GetCurrentFrame()), &resizeInfo);
		}
		if (p->onlyOnce) {
			if (p->anim.Finished()) {
				p->collider->to_delete = true;
				p->to_delete = true;
			}
		}

		// Handle particle fx here ?
		if (p->firstSound) {
			App->audio->PlayFx(p->efxIndex);
			p->firstSound = false;
		}
	}

	return UPDATE_CONTINUE;
}

void ModuleParticles::AddParticle(const Particle& particle, float x, float y,float z)
{
	Particle* p = new Particle(particle);
	p->position = { x,(y - (p->anim.GetCurrentFrame().h / 2)),z };
	if (p->collider != nullptr) {
		p->collider->rect.x =(int) x;
		p->collider->rect.y =(int) y;
		p->collider->z = z;
	}
	active.push_back(p);
}

void ModuleParticles::AddParticle(const Particle& particle, float x, float y, float z, fPoint unitaryVector) {
	Particle* p = new Particle(particle);
	p->position = { x,(y - (p->anim.GetCurrentFrame().h / 2)),z };
	p->pathVector = unitaryVector;
	if (p->collider != nullptr) {
		p->collider->rect.x = (int)x;
		p->collider->rect.y = (int)y;
		p->collider->z = z;
	}
	active.push_back(p);
}

void ModuleParticles::OnCollision(Collider* col,Collider* other) {
	for (list<Particle*>::iterator it = active.begin(); it != active.end(); ++it) {
		if ((*it)->collider == col) {
			(*it)->collider->to_delete = true;
			(*it)->to_delete = true;
			//AddParticle(explosion,(*it)->position.x,(*it)->position.y);
		}
	}
}

// -------------------------------------------------------------
// -------------------------------------------------------------

Particle::Particle()
{}

// TODO 3: Fill in a copy constructor
Particle::Particle(const Particle& p) : anim(p.anim), position(p.position),efxIndex(p.efxIndex),speed(p.speed),onlyOnce(p.onlyOnce),texture(p.texture)
{
	if (p.collider == nullptr) {
		collider = nullptr;
	}
	else
	{
		collider = App->collision->AddCollider(p.collider->rect,p.collider->z,p.collider->speed, p.collider->type, p.collider->callback);
	}
}

Particle::~Particle()
{
}

void Particle::Update()
{
	//Player Shoots
	if (collider->type == LASER) {
		position.z += speed*App->time->GetDeltaTime();
		if (position.z >= MAX_Z) {
			collider->to_delete = true;
			to_delete = true;
		}

		//shadow manage only
		float screenY = App->floor->GetFloorPositionFromZ(position.z);

		float scale = CLIPDISTANCE / (CLIPDISTANCE + position.z);
		if (firstUpdate) {
			reduction = App->player->GetRelativeWorldPosition();
			firstUpdate = false;
		}

		screenPoint.w = anim.GetCurrentFrame().w*scale;
		screenPoint.h = anim.GetCurrentFrame().h*scale;
		screenPoint.x = (position.x - (screenPoint.w*reduction.x*(1-scale)));
		
		if (position.y < screenY) {
			screenPoint.y = screenY;
		}
		else
		{
			screenPoint.y = position.y;
		}

	}

	//Explosion
	if (collider != nullptr) {
		collider->rect = screenPoint;
		collider->z = position.z;
	}

	//Enemy Shoot
	if (collider->type == ENEMY_SHOOT) {
		position.x += speed * pathVector.x * App->time->GetDeltaTime();
		position.y += speed * pathVector.y * App->time->GetDeltaTime();
		position.z += speed * pathVector.z * App->time->GetDeltaTime();

		float screenY = App->floor->GetFloorPositionFromZ(position.z);

		if (position.z <= 0) {
			collider->to_delete = true;
			to_delete = true;
		}

		float scale = 1 - (screenY / App->floor->horizon.y);

		App->shadows->DrawShadow(position.x, screenY, scale);

		screenY += position.y*scale;

		screenPoint.w = anim.GetCurrentFrame().w*scale;
		screenPoint.h = anim.GetCurrentFrame().h*scale;
		screenPoint.x = position.x;
		screenPoint.y = screenY;
	}
}

