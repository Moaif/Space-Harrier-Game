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

	bool Start()override;
	bool CleanUp()override;

	void DrawShadow(const float& x,const float& y,const float& scale);

private:
	SDL_Texture* shadow=nullptr;
	SDL_Rect shadowRect;
};

#endif // !_MODULESHADOW_H_

