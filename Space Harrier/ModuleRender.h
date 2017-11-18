#ifndef __MODULERENDER_H__
#define __MODULERENDER_H__

#include "Module.h"
#include "Globals.h"
#include "Point.h"
#include <queue>

#define STAGE_SPEED_MAX 2
#define STAGE_SPEED_MIN -2
#define BACKGROUND_SPEED_MAX 1
#define BACKGROUND_SPEED_MIN -1

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;
class Font;

struct BlitStruct
{
	SDL_Texture* texture;
	int x;
	int y;
	int z;
	SDL_Rect section;
	SDL_Rect blitSection;
	bool sectionNull;
	bool blitSectionNull;
};

struct CompareDepth {
	bool operator()(BlitStruct const & p1, BlitStruct const & p2) {
		return p1.z < p2.z;
	}
};

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

	void AddToBlitBuffer(SDL_Texture* texture, int x, int y,int z, SDL_Rect* section, SDL_Rect* blitSection);
	bool Blit(SDL_Texture* texture, int x, int y, SDL_Rect* section, SDL_Rect* blitSection);
	bool Print(const Font* font,int x,int y,std::string message);
	bool DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
	bool DrawQuads(const SDL_Rect rects[], int count, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	SDL_Rect ToScreenPointBasic(float x, float y, float z, SDL_Rect* section);

private:
	float DepthScale(float z);


public:
	SDL_Renderer* renderer = nullptr;
	int nearClippingPlane;

private:
	//Depth buffer
	std::priority_queue<BlitStruct,std::vector<BlitStruct>,CompareDepth> blitQueue;


};

#endif // __MODULERENDER_H__