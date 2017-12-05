#ifndef _MODULEENEMY_H_
#define _MODULEENEMY_H_

#include "Globals.h"
#include "Module.h"
#include <list>
#include "Enemy.h"
#include <map>


struct SDL_Texture;

class ModuleEnemy: public Module {
public:
	ModuleEnemy();
	~ModuleEnemy();

	bool Start();
	update_status PreUpdate(); 
	update_status Update(); 
	bool CleanUp();

	void AddEnemy(const Enemy& enemy, const float& x, const float& y, const float& z,Enemy* father=nullptr);
	Enemy* GetById(string id);

private:

	SDL_Texture* enemiesTexture = nullptr;
	SDL_Texture* trees = nullptr;
	SDL_Texture* trees3 = nullptr;
	SDL_Texture* rocks = nullptr;
	SDL_Texture* dragonTexture = nullptr;
	list<Enemy*> active;
	list<Enemy*> prototypePartsClearList;
	list<Enemy*> activeBodyParts;
	map<string, Enemy*> enemies;
};

#endif // !_MODULEENEMY_H_

