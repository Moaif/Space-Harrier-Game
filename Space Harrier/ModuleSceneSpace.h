#ifndef __MODULESCENESPACE_H__
#define __MODULESCENESPACE_H__

#include "Module.h"

struct SDL_Texture;
class Font;

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
	float x,x2;
private:
	const Font* blue;
};

#endif // __MODULESCENESPACE_H__