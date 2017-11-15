#include <math.h>
#include "ModuleParticles.h"
#include "Application.h"
#include "ModuleAudio.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"

#include "SDL/include/SDL_timer.h"

ModuleParticles::ModuleParticles()
{}

ModuleParticles::~ModuleParticles()
{}

// Load assets
bool ModuleParticles::Start()
{
	LOG("Loading particles");
	graphics = App->textures->Load("assets/Shoots.png");


	// Creating shoot particle
	laser.anim.frames.push_back({ 1, 1, 91, 61 });
	laser.anim.frames.push_back({ 95, 0, 91, 61 });
	laser.anim.frames.push_back({ 188, 1, 91, 61 });
	laser.anim.frames.push_back({ 284, 0, 91, 61 });
	laser.anim.randFrame = true;
	laser.anim.speed = 0.01f;
	laser.efxIndex = App->audio->LoadFx("assets/laser.wav");
	laser.speed = Z_SPEED;
	laser.collider = new Collider({ 0,0,91,61 },1,LASER,this);


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
	App->textures->Unload(graphics);

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
			App->renderer->AddToBlitBuffer(graphics, (int)p->position.x, (int)p->position.y, (int)p->position.z, &(p->anim.GetCurrentFrame()), nullptr);
		}
		else
		{
			App->renderer->AddToBlitBuffer(graphics, (int)p->screenPoint.x, (int)p->screenPoint.y, (int)p->position.z, &(p->anim.GetCurrentFrame()), &(p->screenPoint));
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
Particle::Particle(const Particle& p) : anim(p.anim), position(p.position),efxIndex(p.efxIndex),speed(p.speed),onlyOnce(p.onlyOnce)
{
	if (p.collider == nullptr) {
		collider = nullptr;
	}
	else
	{
		collider = App->collision->AddCollider(p.collider->rect,p.collider->z, p.collider->type, p.collider->callback);
	}
}

Particle::~Particle()
{
}

void Particle::Update()
{
	position.z += Z_SPEED;
	if (position.z >= MAX_Z) {
		collider->to_delete = true;
		to_delete = true;
	}
	screenPoint = App->renderer->ToScreenPointBasic(position.x,position.y,position.z,&(anim.GetCurrentFrame()));

	if (collider != nullptr) {
		collider->rect = screenPoint;
		collider->z = position.z;
	}
}

