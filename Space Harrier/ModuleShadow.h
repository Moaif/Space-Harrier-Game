#ifndef _MODULESHADOW_H_
#define _MODULESHADOW_H_

#include "Globals.h"
#include "Module.h"

struct SDL_Texture;
struct SDL_Rect;

class ModuleShadow :public Module {
public:
	ModuleShadow();
	~ModuleShadow();

	bool Start();
	bool CleanUp();

	void DrawShadow(int x, int y,float scale);

private:
	SDL_Texture* shadow;
	SDL_Rect shadowRect;
};

#endif // !_MODULESHADOW_H_

