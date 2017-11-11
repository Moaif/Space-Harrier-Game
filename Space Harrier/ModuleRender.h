#ifndef __MODULERENDER_H__
#define __MODULERENDER_H__

#include "Module.h"
#include "Globals.h"
#include "Point.h"
#include <queue>

#define ALPHA_DISTANCE_MAX 20.0f*SCREEN_SIZE
#define ALPHA_DISTANCE_MIN 5.0f*SCREEN_SIZE
#define ALPHA_SIZE_MAX 13.3f*SCREEN_SIZE
#define ALPHA_SIZE_MIN 3.32f*SCREEN_SIZE

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
	bool DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera = true);
	bool DrawFloor(SDL_Texture* texture);
	SDL_Rect ToScreenPoint(float x,float y,float z, SDL_Rect* section);
	void SetAlphaLineParametersPercentual(float percent);
	void SetXSpeed(float value);

private:
	void ModuleRender::DrawAlphaLines();
	float DepthScale(float z);


public:
	SDL_Renderer* renderer = nullptr;
	SDL_Rect camera;
	iPoint horizon;
	int nearClippingPlane;

private:
	std::priority_queue<BlitStruct,std::vector<BlitStruct>,CompareDepth> blitQueue;
	float increasePixelIteration=0.0f;
	float alphaLineDistanceStart;
	float alphaLineSizeStart;
	float xSpeed = 0.0f;
	float ySpeed = 60.0f;
	float alphaLineDistance;
	float alphaLineSize;
	int alphaLineIteration=0;

};

#endif // __MODULERENDER_H__