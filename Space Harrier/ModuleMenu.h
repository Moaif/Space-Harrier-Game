#ifndef __MODULEMENU_H__
#define __MODULEMENU_H__

#include "Module.h"
#include "Animation.h"

struct SDL_Texture;

class ModuleMenu : public Module
{
public:
	ModuleMenu(bool active = true);
	~ModuleMenu();

	bool Start();
	update_status Update();
	bool CleanUp();

private:
	SDL_Texture* background = nullptr;
	uint fx = 0;
	Animation robot;

};

#endif // __MODULEMENU_H__