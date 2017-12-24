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
	const Enemy* enemy;
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

	bool Start()override;
	bool Restart()override;
	update_status Update()override;
	bool CleanUp()override;

	bool ModuleScene::LoadJson(const string& path);
	void Win();
	void End();

public:
	//For UI module
	string  stageName;
	int currentStage;
	//For multiple enemy Boss
	int numEnemies = 0;
private:
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
	//Initial sound
	uint fx = 0;
};

#endif // __MODULESCENE_H__