#ifndef __APPLICATION_CPP__
#define __APPLICATION_CPP__

#include<list>
#include "Globals.h"
#include "Module.h"

class ModuleAnim;
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
class ModuleUI;

// Game modules ---
class ModulePlayer;
class ModuleSceneSega;
class ModuleMenu;
class ModuleScene;

class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	bool Restart();

public:
	ModuleAnim* anim = nullptr;
	ModuleRender* renderer=nullptr;
	ModuleWindow* window=nullptr;
	ModuleTextures* textures=nullptr;
	ModuleInput* input=nullptr;
	ModuleAudio* audio=nullptr;
	ModuleFadeToBlack* fade=nullptr;
	ModuleCollision* collision=nullptr;
	ModuleParticles* particles=nullptr;
	ModuleEnemy* enemies=nullptr;
	ModuleFont* fonts=nullptr;
	ModuleFloor* floor=nullptr;
	ModuleShadow* shadows=nullptr;
	ModuleTime* time=nullptr;
	ModuleUI* ui=nullptr;

	// Game modules ---
	ModulePlayer* player=nullptr;
	ModuleSceneSega* sega=nullptr;
	ModuleMenu* menu=nullptr;
	ModuleScene* scene=nullptr;

	bool playing;

private:

	std::list<Module*> modules;
};

extern Application* App;

#endif // __APPLICATION_CPP__