#ifndef _MODULEENEMY_H_
#define _MODULEENEMY_H_

#include "Globals.h"
#include "Module.h"
#include <list>
#include "Enemy.h"


struct SDL_Texture;

class ModuleEnemy: public Module {
public:
	ModuleEnemy();
	~ModuleEnemy();

	bool Start();
	update_status PreUpdate(); 
	update_status Update(); 
	bool CleanUp();
	void OnCollision(Collider* col, Collider* other);

	void AddEnemy(const Enemy& particle, float x, float y,float z);

private:

	SDL_Texture* graphics = nullptr;
	SDL_Texture* trees = nullptr;
	std::list<Enemy*> active;

public:
	Enemy* tree;
};

#endif // !_MODULEENEMY_H_

