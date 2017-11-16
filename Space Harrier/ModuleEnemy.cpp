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

	tree = new Obstacle();
	tree->anim.frames.push_back({ 206,48,44,163 });
	tree->collider = new Collider({ 0,0,0,0 }, MAX_Z, ENEMY, this);

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
			App->renderer->AddToBlitBuffer(trees, p->position.x, p->position.y, (int)p->position.z, &(p->anim.GetCurrentFrame()),nullptr);
		}
		else {
			App->renderer->AddToBlitBuffer(trees, p->screenPoint.x, p->screenPoint.y, (int)p->position.z, &(p->anim.GetCurrentFrame()), &(p->screenPoint));
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
			--((*it)->hits);
			if ((*it)->hits <=0) {
				(*it)->collider->to_delete = true;
				(*it)->to_delete = true;
			}
		}
	}
}