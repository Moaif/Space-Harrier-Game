#ifndef __MODULESCENEINTRO_H__
#define __MODULESCENEINTRO_H__

#include "Module.h"
#include "Animation.h"

struct SDL_Texture;
class Font;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(bool active = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();

public:
	
	SDL_Texture* background = nullptr;
	uint fx = 0;
	Animation robot;
private:
	const Font* red;
	const Font* blue;
	const Font* green;
	const Font * yellow;
	int v=0;
};

#endif // __MODULESCENEINTRO_H__