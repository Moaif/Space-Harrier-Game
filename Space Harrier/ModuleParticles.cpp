#include <math.h>
#include "ModuleParticles.h"
#include "Application.h"
#include "ModuleAudio.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"
#include "ModuleTime.h"
#include "ModulePlayer.h"

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


	// TODO 12: Create a new "Explosion" particle 
	// audio: rtype/explosion.wav
	// coords: {274, 296, 33, 30}; {313, 296, 33, 30}; {346, 296, 33, 30}; {382, 296, 33, 30}; {419, 296, 33, 30}; {457, 296, 33, 30};
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

void ModuleParticles::AddParticle(const Particle& particle, float x, float y)
{
	Particle* p = new Particle(particle);
	p->position = { x,(y - (p->anim.GetCurrentFrame().h / 2)) };
	if (p->collider != nullptr) {
		p->collider->rect.x =(int) x;
		p->collider->rect.y =(int) y;
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
		float scale = CLIPDISTANCE / (CLIPDISTANCE + position.z);
		if (firstUpdate) {
			reduction = App->player->GetRelativeWorldPosition();
			firstUpdate = false;
		}
		screenPoint.w = anim.GetCurrentFrame().w*scale;
		screenPoint.h = anim.GetCurrentFrame().h*scale;
		float tempY = position.y - anim.GetCurrentFrame().h/2 * (reduction.y - 1)  *0.75;//0.75 is a coeficient to reduce the initial y pos gathered by test
		screenPoint.x = (position.x - (screenPoint.w*reduction.x*(1-scale)));
		screenPoint.y = (tempY + (screenPoint.h*(reduction.y)*pow((1-scale),2)));
		screenPoint.y += screenPoint.h/2 * (reduction.y -1) ;
	}

	//Explosion
	if (collider != nullptr) {
		collider->rect = screenPoint;
		collider->z = position.z;
	}

	//Enemy Shoot
	if (collider->type == ENEMY_SHOOT) {

	}
}

