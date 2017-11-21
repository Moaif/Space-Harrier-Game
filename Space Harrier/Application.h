#ifndef __APPLICATION_CPP__
#define __APPLICATION_CPP__

#include<list>
#include "Globals.h"
#include "Module.h"

class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleAudio;
class ModuleFadeToBlack;
class ModuleCollision;
class ModuleParticles;
class ModuleFont;
class ModuleEnemy;
class ModuleFloor;
class ModuleShadow;
class ModuleTime;

// Game modules ---
class ModulePlayer;
class ModuleSceneIntro;
class ModuleScene;

class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

public:
	ModuleRender* renderer;
	ModuleWindow* window;
	ModuleTextures* textures;
	ModuleInput* input;
	ModuleAudio* audio;
	ModuleFadeToBlack* fade;
	ModuleCollision* collision;
	ModuleParticles* particles;
	ModuleEnemy* enemies;
	ModuleFont* fonts;
	ModuleFloor* floor;
	ModuleShadow* shadows;
	ModuleTime* time;

	// Game modules ---
	ModulePlayer* player;
	ModuleSceneIntro* scene_intro;
	ModuleScene* scene_space;

	bool playing;

private:

	std::list<Module*> modules;
};

extern Application* App;

#endif // __APPLICATION_CPP__