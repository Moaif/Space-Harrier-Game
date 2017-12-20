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

	bool Start()override;
	update_status Update()override;
	bool CleanUp()override;

private:
	//Text
	const Font* red=nullptr;
	int redFontLineReference;
	float textTimer;
	//Images
	SDL_Texture* content = nullptr;
	SDL_Rect background;
	Animation robot;
	Animation* robotPointer=nullptr;
	Animation eye;
	Animation* eyePointer=nullptr; //TODO: check if posible to do double & to cast normal animation into Animation**
	Animation title;
	Animation* titlePointer=nullptr;
	int titleMult;
	SDL_Rect finalTitle;
	Animation man;
	//Sound
	uint fx = 0;
	//Repetition variable
	bool firstPlay=true;
	//Static variables
	static const float TEXT_INTERVAL;

};

#endif // __MODULEMENU_H__