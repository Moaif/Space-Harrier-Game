#ifndef __MODULERENDER_H__
#define __MODULERENDER_H__

#include "Module.h"
#include "Globals.h"
#include "Point.h"

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

class ModuleRender : public Module
{
public:
	ModuleRender();
	~ModuleRender();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	bool Blit(SDL_Texture* texture, int x, int y, SDL_Rect* section, SDL_Rect* blitSection);
	bool DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera = true);
	SDL_Rect ToScreenPoint(float x,float y,float z, SDL_Rect* section);

private:
	float DepthScale(float z);

public:
	SDL_Renderer* renderer = nullptr;
	SDL_Rect camera;
	iPoint horizon;
	int nearClippingPlane;
};

#endif // __MODULERENDER_H__