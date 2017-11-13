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
	laser.speed = 0.5f;
	laser.collider = App->collision->AddCollider({-16,-12,91,61},LASERS,this);//Is generated out of screen

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
		SDL_Rect screenPoint = App->renderer->ToScreenPoint(p->position.x,p->position.y,p->position.z,&(p->anim.GetCurrentFrame()));
		App->renderer->AddToBlitBuffer(graphics, screenPoint.x, screenPoint.y,(int)p->position.z, &(p->anim.GetCurrentFrame()),&screenPoint);
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

void ModuleParticles::AddParticle(const Particle& particle, float x, float y)
{
	// TODO 4: Fill in a method to create an instance of a prototype particle	
	Particle* p = new Particle(particle);
	p->position = { x,y };
	if (p->collider != nullptr) {
		p->collider->rect.x =(int) x;
		p->collider->rect.y =(int) y;
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
	if (position.z >= MAX_Z) {
		to_delete = true;
	}
	position.z += speed;
	if (collider != nullptr) {
		float temp1 = (anim.GetCurrentFrame().w / position.z);
		float temp2 = (anim.GetCurrentFrame().h / position.z);
		collider->rect.x +=(int) ((anim.GetCurrentFrame().w-temp1)/2);
		collider->rect.y +=(int) ((anim.GetCurrentFrame().h - temp2)/2);
		collider->rect.w = (int)temp1;
		collider->rect.h = (int)temp2;
	}
}

