#ifndef __MODULESCENEH__
#define __MODULESCENE_H__

#include "Module.h"
#include <list>

struct SDL_Texture;
class Font;

class Enemy;

struct EnemyInstantiate
{
	int x;
	int y;
	Enemy* enemy;
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

public:
	

	Animation tree;
	float x,x2;
private:
	const Font* blue;
	string backgroundPath;
	string stagePath;
	string floorPath;
	SDL_Texture* background = nullptr;
	SDL_Texture* stage = nullptr;
	SDL_Texture* floor = nullptr;
	std::list<std::list<EnemyInstantiate>> elements;
};

#endif // __MODULESCENE_H__