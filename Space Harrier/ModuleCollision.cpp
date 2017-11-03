#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"

using namespace std;

ModuleCollision::ModuleCollision()
{
	hits[PLAYER][PLAYER] = false;
	hits[PLAYER][LASERS] = false;
	hits[PLAYER][WALLS] = true;
	hits[LASERS][PLAYER] = false;
	hits[LASERS][LASERS] = false;
	hits[LASERS][WALLS] = true;
	hits[WALLS][PLAYER] = true;
	hits[WALLS][LASERS] = true;
	hits[WALLS][WALLS] = false;

}

// Destructor
ModuleCollision::~ModuleCollision()
{}

update_status ModuleCollision::PreUpdate()
{
	// Remove all colliders scheduled for deletion
	for (list<Collider*>::iterator it = colliders.begin(); it != colliders.end();)
	{
		if ((*it)->to_delete == true)
		{
			RELEASE(*it);
			it = colliders.erase(it);
		}
		else
			++it;
	}

	return UPDATE_CONTINUE;
}

update_status ModuleCollision::Update()
{
	// TODO 8: Check collisions between all colliders. 
	// After making it work, review that you are doing the minumum checks possible
	for (list<Collider*>::iterator it = colliders.begin(); it != colliders.end(); ++it) {
		for (list<Collider*>::iterator it2 = it; it2 != colliders.end(); ++it2) {
			if ((*it)->CheckCollision((*it2)->rect)) {
				if (hits[(*it)->type][(*it2)->type]) {
					(*it)->callback->OnCollision((*it),(*it2));
					(*it2)->callback->OnCollision((*it2),(*it));
				}
			}
		}
	}

	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(debug == true)
		DebugDraw();

	return UPDATE_CONTINUE;
}

void ModuleCollision::DebugDraw()
{
	for (list<Collider*>::iterator it = colliders.begin(); it != colliders.end(); ++it)
		App->renderer->DrawQuad((*it)->rect, 255, 0, 0, 80);
}

// Called before quitting
bool ModuleCollision::CleanUp()
{
	LOG("Freeing all colliders");

	for (list<Collider*>::iterator it = colliders.begin(); it != colliders.end(); ++it)
		RELEASE(*it);

	colliders.clear();

	return true;
}

Collider* ModuleCollision::AddCollider(const SDL_Rect& rect,CollisionType type,Module* callback)
{
	Collider* ret = new Collider(rect,type,callback);

	colliders.push_back(ret);

	return ret;
}

// -----------------------------------------------------

bool Collider::CheckCollision(const SDL_Rect& r) const
{
	// TODO 7: Create by hand (avoid consulting the internet) a simple collision test
	// Return true if rectangle argument "r" if intersecting with "this->rect"
	bool xHit = true;
	bool yHit = true;

	//Xcollision
	if (r.x > (this->rect.x + this->rect.w) || (r.x+r.w) < this->rect.x) {
		xHit = false;
	}

	//Ycollision
	if (r.y > (this->rect.y + this->rect.h) || (r.y + r.h) < this->rect.y) {
		yHit = false;
	}

	return (xHit && yHit);
}
