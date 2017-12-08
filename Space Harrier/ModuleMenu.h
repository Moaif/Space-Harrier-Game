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
	const Font* red;
	int redFontLineReference;
	float textTimer;
	SDL_Texture* content = nullptr;
	uint fx = 0;
	SDL_Rect background;
	Animation robot;
	Animation eye;
	Animation title;
	int titleMult;
	SDL_Rect finalTitle;
	Animation man;
	static const float TEXT_INTERVAL;

};

#endif // __MODULEMENU_H__