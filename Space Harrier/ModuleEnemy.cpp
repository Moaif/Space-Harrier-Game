#include "Application.h"
#include "ModuleEnemy.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"
#include "Obstacle.h"
#include "Jellyfish.h"
#include "Drone.h"
#include "Fly.h"



ModuleEnemy::ModuleEnemy()
{
}

ModuleEnemy::~ModuleEnemy()
{}

// Load assets
bool ModuleEnemy::Start()
{
	LOG("Loading Enemies");
	enemiesTexture = App->textures->Load("assets/Enemies.png");
	trees = App->textures->Load("assets/Arboles.png");
	rocks = App->textures->Load("assets/models.png");


	Enemy* tree = new Obstacle(trees);
	tree->anim.frames.push_back({ 206,48,44,163 });
	tree->collider = new Collider({ 0,0,0,0 }, MAX_Z,0, ENEMY, this);
	tree->shadow = false;
	enemies["tree1"] = tree;
	

	Enemy* rock = new Obstacle(rocks);
	rock->anim.frames.push_back({191,71,61,39});
	rock->collider = new Collider({0,0,0,0},MAX_Z,0,ENEMY,this);
	enemies["rock1"] = rock;

	Enemy* jellyfish = new Jellyfish(200.0f,enemiesTexture);
	jellyfish->anim.frames.push_back({0,43,80,90});
	jellyfish->anim.frames.push_back({ 83,43,80,90 });
	jellyfish->anim.frames.push_back({ 170,43,80,90 });
	jellyfish->collider = new Collider({ 0,0,0,0 }, MAX_Z, 0, ENEMY, this);
	jellyfish->speed = { 20.0f, 200.0f, 0.0f };
	enemies["jelly1"] = jellyfish;

	Enemy* jellyfish2 = new Jellyfish(200.0f, enemiesTexture);
	jellyfish2->anim.frames.push_back({ 0,43,80,90 });
	jellyfish2->anim.frames.push_back({ 83,43,80,90 });
	jellyfish2->anim.frames.push_back({ 170,43,80,90 });
	jellyfish2->collider = new Collider({ 0,0,0,0 }, MAX_Z, 0, ENEMY, this);
	jellyfish2->speed = { -20.0f, 200.0f, 0.0f };
	enemies["jelly2"] = jellyfish2;

	Enemy* jellyfish3 = new Jellyfish(200.0f, enemiesTexture);
	jellyfish3->anim.frames.push_back({ 0,43,80,90 });
	jellyfish3->anim.frames.push_back({ 83,43,80,90 });
	jellyfish3->anim.frames.push_back({ 170,43,80,90 });
	jellyfish3->collider = new Collider({ 0,0,0,0 }, MAX_Z, 0, ENEMY, this);
	jellyfish3->speed = { 20.0f, 100.0f, 0.0f };
	enemies["jelly3"] = jellyfish3;

	Enemy* jellyfish4 = new Jellyfish(200.0f, enemiesTexture);
	jellyfish4->anim.frames.push_back({ 0,43,80,90 });
	jellyfish4->anim.frames.push_back({ 83,43,80,90 });
	jellyfish4->anim.frames.push_back({ 170,43,80,90 });
	jellyfish4->collider = new Collider({ 0,0,0,0 }, MAX_Z, 0, ENEMY, this);
	jellyfish4->speed = { -20.0f, 100.0f, 0.0f };
	enemies["jelly4"] = jellyfish4;

	Enemy* drone = new Drone(0,enemiesTexture);
	drone->anim.frames.push_back({0,7,80,34});
	drone->anim.frames.push_back({ 84,7,80,34 });
	drone->anim.frames.push_back({ 168,7,80,34 });
	drone->collider = new Collider({0,0,0,0},0,0,ENEMY,this);
	drone->speed = { 100.0f,200.0f,10.0f };
	enemies["drone1"] = drone;

	Enemy* drone2 = new Drone(1, enemiesTexture);
	drone2->anim.frames.push_back({ 0,7,80,34 });
	drone2->anim.frames.push_back({ 84,7,80,34 });
	drone2->anim.frames.push_back({ 168,7,80,34 });
	drone2->collider = new Collider({ 0,0,0,0 }, 0, 0, ENEMY, this);
	drone2->speed = { 120.0f,0.0f,10.0f };
	enemies["drone2"] = drone2;

	Enemy* drone3 = new Drone(1, enemiesTexture);
	drone3->anim.frames.push_back({ 0,7,80,34 });
	drone3->anim.frames.push_back({ 84,7,80,34 });
	drone3->anim.frames.push_back({ 168,7,80,34 });
	drone3->collider = new Collider({ 0,0,0,0 }, 0, 0, ENEMY, this);
	drone3->speed = { -120.0f,0.0f,10.0f };
	enemies["drone3"] = drone3;

	Enemy* drone4 = new Drone(2, enemiesTexture);
	drone4->anim.frames.push_back({ 0,7,80,34 });
	drone4->anim.frames.push_back({ 84,7,80,34 });
	drone4->anim.frames.push_back({ 168,7,80,34 });
	drone4->collider = new Collider({ 0,0,0,0 }, MAX_Z, 0, ENEMY, this);
	drone4->speed = { -50.0f,120.0f,-5.0f };
	enemies["drone4"] = drone4;

	Enemy* drone5 = new Drone(2, enemiesTexture);
	drone5->anim.frames.push_back({ 0,7,80,34 });
	drone5->anim.frames.push_back({ 84,7,80,34 });
	drone5->anim.frames.push_back({ 168,7,80,34 });
	drone5->collider = new Collider({ 0,0,0,0 }, MAX_Z, 0, ENEMY, this);
	drone5->speed = { 50.0f,120.0f,-5.0f };
	enemies["drone5"] = drone5;

	Enemy* drone6 = new Drone(3, enemiesTexture);
	drone6->anim.frames.push_back({ 0,7,80,34 });
	drone6->anim.frames.push_back({ 84,7,80,34 });
	drone6->anim.frames.push_back({ 168,7,80,34 });
	drone6->collider = new Collider({ 0,0,0,0 }, 0, 0, ENEMY, this);
	drone6->speed = { 100.0f,200.0f,10.0f };
	enemies["drone6"] = drone6;

	Enemy* drone7 = new Drone(3, enemiesTexture);
	drone7->anim.frames.push_back({ 0,7,80,34 });
	drone7->anim.frames.push_back({ 84,7,80,34 });
	drone7->anim.frames.push_back({ 168,7,80,34 });
	drone7->collider = new Collider({ 0,0,0,0 }, 0, 0, ENEMY, this);
	drone7->speed = { -100.0f,200.0f,10.0f };
	enemies["drone7"] = drone7;


	Enemy* fly = new Fly(0,enemiesTexture);
	fly->anim.frames.push_back({8,144,78,32});
	fly->collider = new Collider({0,0,0,0},MAX_Z,0,ENEMY,this);
	fly->speed = {25.0f, -70.0f, -2.5f};
	enemies["fly1"] = fly;

	Enemy* fly2 = new Fly(0, enemiesTexture);
	fly2->anim.frames.push_back({ 8,144,78,32 });
	fly2->collider = new Collider({ 0,0,0,0 }, MAX_Z, 0, ENEMY, this);
	fly2->speed = { 0.0f, -70.0f, -2.5f };
	enemies["fly2"] = fly2;

	Enemy* fly3 = new Fly(0, enemiesTexture);
	fly3->anim.frames.push_back({ 8,144,78,32 });
	fly3->collider = new Collider({ 0,0,0,0 }, MAX_Z, 0, ENEMY, this);
	fly3->speed = { -25.0f, -70.0f, -2.5f };
	enemies["fly3"] = fly3;

	Enemy* fly4 = new Fly(1, enemiesTexture);
	fly4->anim.frames.push_back({ 8,144,78,32 });
	fly4->collider = new Collider({ 0,0,0,0 }, MAX_Z, 0, ENEMY, this);
	fly4->speed = { 35.0f, -60.0f, -2.5f };
	enemies["fly4"] = fly4;

	Enemy* fly5 = new Fly(1, enemiesTexture);
	fly5->anim.frames.push_back({ 8,144,78,32 });
	fly5->collider = new Collider({ 0,0,0,0 }, MAX_Z, 0, ENEMY, this);
	fly5->speed = { -35.0f, -60.0f, -2.5f };
	enemies["fly5"] = fly5;

	return true;
}

// Unload assets
bool ModuleEnemy::CleanUp()
{
	LOG("Unloading particles");
	App->textures->Unload(enemiesTexture);

	for (list<Enemy*>::iterator it = active.begin(); it != active.end(); ++it)
		RELEASE(*it);

	active.clear();

	return true;
}

// PreUpdate to clear up all dirty particles
update_status ModuleEnemy::PreUpdate()
{
	for (list<Enemy*>::iterator it = active.begin(); it != active.end();)
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
update_status ModuleEnemy::Update()
{
	for (list<Enemy*>::iterator it = active.begin(); it != active.end(); ++it)
	{
		Enemy* p = *it;

		p->Update();
		if (p->screenPoint.h ==0 && p->screenPoint.w ==0) {
			App->renderer->AddToBlitBuffer(p->texture, p->position.x, p->position.y, p->position.z, &(p->anim.GetCurrentFrame()),nullptr);
		}
		else {
			resizeStruct resizeInfo = { p->screenPoint.w,p->screenPoint.h };
			App->renderer->AddToBlitBuffer(p->texture, (float)p->screenPoint.x, (float)p->screenPoint.y, p->position.z, &(p->anim.GetCurrentFrame()), &resizeInfo);
		}
	}

	return UPDATE_CONTINUE;
}

void ModuleEnemy::AddEnemy(const Enemy& enemy, float x, float y, float z)
{
	Enemy* p = enemy.Copy(x, y, z);
	active.push_back(p);
}

void ModuleEnemy::OnCollision(Collider* col, Collider* other) {
	for (list<Enemy*>::iterator it = active.begin(); it != active.end(); ++it) {
		if ((*it)->collider == col && (*it)->destructible) {
			if ((*it)->father == nullptr) {
				--((*it)->hits);
				if ((*it)->hits <= 0) {
					(*it)->collider->to_delete = true;
					(*it)->to_delete = true;
				}
			}
			else
			{
				--((*it)->father->hits);
				if ((*it)->father->hits <= 0) {
					for (list<Enemy*>::iterator cIt = (*it)->father->childs.begin(); cIt != (*it)->father->childs.end(); ++cIt) {
						(*cIt)->collider->to_delete = true;
						(*cIt)->to_delete = true;
					}
					(*it)->father->to_delete = true;
				}
			}
		}
	}
}

Enemy* ModuleEnemy::GetById(string id) {
	map<string, Enemy*>::iterator it = enemies.find(id);
	if (it != enemies.end()) {
		return enemies[id];
	}
	return nullptr;
}