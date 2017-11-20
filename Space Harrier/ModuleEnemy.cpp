#include "Application.h"
#include "ModuleEnemy.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"
#include "Obstacle.h"



ModuleEnemy::ModuleEnemy()
{
}

ModuleEnemy::~ModuleEnemy()
{}

// Load assets
bool ModuleEnemy::Start()
{
	LOG("Loading particles");
	graphics = App->textures->Load("assets/Shoots.png");
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

	return true;
}

// Unload assets
bool ModuleEnemy::CleanUp()
{
	LOG("Unloading particles");
	App->textures->Unload(graphics);

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
	Enemy* p = enemy.Copy();
	p->position = { x,y,z };
	p->collider->rect.x = (int)x;
	p->collider->rect.y = (int)y;
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