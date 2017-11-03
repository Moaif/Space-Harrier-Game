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
	graphics = App->textures->Load("assets/particles.png");


	// TODO 2: Create a prototype for the laser particle
	// audio: rtype/laser.wav
	// coords: {232, 103, 16, 12}; {249, 103, 16, 12};
	laser.anim.frames.push_back({ 232, 103, 16, 12 });
	laser.anim.frames.push_back({ 249, 103, 16, 12 });
	laser.efxIndex = App->audio->LoadFx("assets/laser.wav");
	laser.speed = 10;
	laser.collider = App->collision->AddCollider({-16,-12,16,12},LASERS,this);

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
		App->renderer->Blit(graphics, p->position.x, p->position.y, &(p->anim.GetCurrentFrame()));
		if (p->onlyOnce) {
			if (p->anim.Finished()) {
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

void ModuleParticles::AddParticle(const Particle& particle, int x, int y)
{
	// TODO 4: Fill in a method to create an instance of a prototype particle	
	Particle* p = new Particle(particle);
	p->position = { x,y };
	if (p->collider != nullptr) {
		p->collider->rect.x = x;
		p->collider->rect.y = y;
	}
	active.push_back(p);
}

void ModuleParticles::OnCollision(Collider* col,Collider* other) {
	for (list<Particle*>::iterator it = active.begin(); it != active.end(); ++it) {
		if ((*it)->collider == col) {
			(*it)->to_delete = true;
			AddParticle(explosion,(*it)->position.x,(*it)->position.y);
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
		collider = App->collision->AddCollider(p.collider->rect, p.collider->type, p.collider->callback);
	}
}

Particle::~Particle()
{
}

void Particle::Update()
{
	// TODO 5: This is the core of the particle logic
	// draw and audio will be managed by ModuleParticle::Update()
	// Note: Set to_delete to true is you want it deleted
	position.x += speed;
	if (collider != nullptr) {
		collider->rect.x = position.x;
	}
}

