#ifndef __MODULESCENESPACE_H__
#define __MODULESCENESPACE_H__

#include "Module.h"

struct SDL_Texture;

class ModuleSceneSpace : public Module
{
public:
	ModuleSceneSpace(bool active = true);
	~ModuleSceneSpace();

	bool Start();
	update_status Update();
	bool CleanUp();

public:
	
	SDL_Texture* background = nullptr;
	SDL_Texture* stage = nullptr;
	SDL_Texture* stars = nullptr;
	SDL_Texture* trees = nullptr;
	SDL_Texture* floor = nullptr;
	Animation tree;
	float z,z2,x,x2;
};

#endif // __MODULESCENESPACE_H__