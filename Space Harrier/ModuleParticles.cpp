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
#include "Explosion.h"

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
	ASSERT(lasers != nullptr,AT("Failed on loading texture"));
	shots = App->textures->Load("assets/Shoots2.png");
	ASSERT(shots != nullptr, AT("Failed on loading texture"));
	exp = App->textures->Load("assets/Explosion.png");
	ASSERT(exp != nullptr, AT("Failed on loading texture"));
	enemLaser = App->textures->Load("assets/EnemyShot.png");
	ASSERT(enemLaser != nullptr, AT("Failed on loading texture"));


	// Creating shoot particle
	laser = new PlayerShoot(lasers);
	laser->anim.frames.push_back({ 0, 0, 56, 38 });
	laser->anim.frames.push_back({ 57, 0, 56, 38 });
	laser->anim.frames.push_back({ 113, 0, 56, 38 });
	laser->anim.frames.push_back({ 171, 0, 56, 38 });
	laser->anim.randFrame = true;
	laser->anim.speed = 3.0f;
	laser->anim.timeBased = false;
	laser->efxIndex = App->audio->LoadFx("assets/music/SFX/PlayerShoot.wav");
	((PlayerShoot*)laser)->reboundEfx = App->audio->LoadFx("assets/music/SFX/ReboundShoot.wav");
	laser->speed = 150.0f;
	prototipeClearList.push_back(laser);

	fire = new EnemyShoot(shots);
	fire->anim.frames.push_back({1,1,51,47});
	fire->anim.frames.push_back({ 56,2,48,44 });
	fire->anim.frames.push_back({ 110,0,50,48 });
	fire->anim.speed = 15.0f;
	fire->anim.timeBased = false;
	fire->efxIndex = App->audio->LoadFx("assets/music/SFX/FireShoot.wav");
	fire->speed = 10.0f;
	prototipeClearList.push_back(fire);

	enemyLaser = new EnemyShoot(enemLaser);
	enemyLaser->anim.frames.push_back({ 8,8,59,59});
	enemyLaser->anim.frames.push_back({ 76,8,59,59 });
	enemyLaser->anim.frames.push_back({ 144,8,59,59 });
	enemyLaser->anim.frames.push_back({ 213,10,59,59 });
	enemyLaser->anim.frames.push_back({ 8,76,59,59 });
	enemyLaser->anim.frames.push_back({ 76,76,59,59 });
	enemyLaser->anim.frames.push_back({ 144,76,59,59 });
	enemyLaser->anim.frames.push_back({ 213,76,59,59 });
	enemyLaser->anim.speed = 15.0f;
	enemyLaser->anim.timeBased = false;
	enemyLaser->efxIndex = App->audio->LoadFx("assets/music/SFX/LaserShoot.wav");
	enemyLaser->speed = 10.0f;
	prototipeClearList.push_back(enemyLaser);

	explosion = new Explosion(400.0f,exp);
	explosion->anim.frames.push_back({2,0,93,67});
	explosion->anim.frames.push_back({ 99,1,92,67 });
	explosion->anim.frames.push_back({ 197,1,94,72 });
	explosion->anim.frames.push_back({ 293,0,98,77 });
	explosion->anim.frames.push_back({ 397,5,95,70 });
	explosion->anim.frames.push_back({ 498,8,96,67 });
	explosion->anim.frames.push_back({ 2,79,98,87 });
	explosion->anim.frames.push_back({ 106,80,94,85 });
	explosion->anim.frames.push_back({ 202,87,97,79 });
	explosion->efxIndex = App->audio->LoadFx("assets/music/SFX/Explosion.wav");
	explosion->anim.speed= 30.0f;
	explosion->speed = 15.0f;
	explosion->onlyOnce = true;
	prototipeClearList.push_back(explosion);

	return true;
}

// Unload assets
bool ModuleParticles::CleanUp()
{
	LOG("Unloading particles");
	App->textures->Unload(lasers);
	App->textures->Unload(shots);
	App->textures->Unload(exp);
	App->textures->Unload(enemLaser);

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
		p->anim.Update();
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
				if (p->collider != nullptr) {
					p->collider->to_delete = true;
				}
				p->to_delete = true;
			}
		}

		if (p->firstSound) {
			App->audio->PlayFx(p->efxIndex);
			p->firstSound = false;
		}
	}

	return UPDATE_CONTINUE;
}

void ModuleParticles::AddParticle(const Particle& particle, const float& x, const float& y, const float& z)
{
	Particle* p = particle.Copy(x,y,z);
	ASSERT(p != nullptr,AT("Failed on creating a new Particle"));
	p->position.y = (y - (p->anim.GetCurrentFrame().h / 2));
	active.push_back(p);
}

