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

	bool Start()override;
	update_status PreUpdate()override; 
	update_status Update()override; 
	bool CleanUp()override;

	void AddEnemy(const Enemy& enemy, const float& x, const float& y, const float& z,Enemy* father=nullptr);
	const Enemy* GetById(string id)const;

private:

	SDL_Texture* graphics = nullptr;
	list<Enemy*> active;
	list<Enemy*> prototypePartsClearList;
	list<Enemy*> activeBodyParts;
	map<string, Enemy*> enemies;

	unsigned int droneStartSFX;
	unsigned int jellyStartSFX;
	unsigned int flyStartSFX;
	unsigned int metalStartSFX;
};

#endif // !_MODULEENEMY_H_

