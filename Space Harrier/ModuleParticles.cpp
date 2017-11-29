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
#include "Particle.h"
#include "PlayerShoot.h"
#include "EnemyShoot.h"

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
	laser = new PlayerShoot(lasers);
	laser->anim.frames.push_back({ 0, 0, 56, 38 });
	laser->anim.frames.push_back({ 57, 0, 56, 38 });
	laser->anim.frames.push_back({ 113, 0, 56, 38 });
	laser->anim.frames.push_back({ 171, 0, 56, 38 });
	laser->anim.randFrame = true;
	laser->anim.speed = 1.0f;
	laser->efxIndex = App->audio->LoadFx("assets/laser.wav");
	laser->speed = 40.0f;
	prototipeClearList.push_back(laser);

	fire = new EnemyShoot(shots);
	fire->anim.frames.push_back({1,1,51,47});
	fire->anim.frames.push_back({ 56,2,48,44 });
	fire->anim.frames.push_back({ 110,0,50,48 });
	fire->anim.speed = 5.0f;
	fire->speed = 80.0f;
	prototipeClearList.push_back(fire);

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

	for (list<Particle*>::iterator it = prototipeClearList.begin(); it != prototipeClearList.end(); ++it) {
		RELEASE(*it);
	}

	prototipeClearList.clear();

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

void ModuleParticles::AddParticle(const Particle* particle, float x, float y,float z)
{
	Particle* p = particle->Copy(x,y,z);
	p->position.y = (y - (p->anim.GetCurrentFrame().h / 2));
	active.push_back(p);
}

void ModuleParticles::AddParticle(const Particle* particle, float x, float y, float z, fPoint unitaryVector) {
	Particle* p = particle->Copy(x, y, z);
	p->position.y = (y - (p->anim.GetCurrentFrame().h / 2));
	p->pathVector = unitaryVector;
	active.push_back(p);
}

