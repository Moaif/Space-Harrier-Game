#ifndef __MODULESCENEH__
#define __MODULESCENE_H__

#include "Module.h"
#include <list>

struct SDL_Texture;
class Font;

class Enemy;

struct EnemyInstantiate
{
	float x;
	float y;
	float z;
	Enemy* enemy;
};

struct DelayList 
{
	float delay;
	list<EnemyInstantiate> list;
};

class ModuleScene : public Module
{
public:
	ModuleScene(bool active = true);
	~ModuleScene();

	bool Start();
	update_status Update();
	bool CleanUp();
	bool ModuleScene::LoadJson(string path);

private:
	const Font* blue;
	string backgroundPath;
	string stagePath;
	string floorPath;
	SDL_Texture* background = nullptr;
	SDL_Texture* stage = nullptr;
	SDL_Texture* floor = nullptr;
	static const float STARTING_DELAY;
	static const float INTERVAL_DELAY;
	float timeElapsed;
	list<DelayList> elements;
	int x, x2;
};

#endif // __MODULESCENE_H__