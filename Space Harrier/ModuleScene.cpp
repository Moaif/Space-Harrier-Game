#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "ModuleScene.h"
#include "ModuleEnemy.h"
#include "ModuleFont.h"
#include "ModuleFloor.h"
#include "json.hpp"
#include <fstream>

using json = nlohmann::json;

ModuleScene::ModuleScene(bool active) : Module(active)
{
	x = 100000.0f;
	x2 = 0.0f;
}

ModuleScene::~ModuleScene()
{}

// Load assets
bool ModuleScene::Start()
{
	LOG("Loading space scene");
	
	background = App->textures->Load("assets/Background3.png");
	stage = App->textures->Load("assets/Stage3.png");
	floor = App->textures->Load("assets/Floor.bmp");
	blue = App->fonts->GetFont("Blue",__FILE__,__LINE__);

	App->player->Enable();
	App->particles->Enable();
	App->collision->Enable();
	App->playing = true;

	App->audio->PlayMusic("assets/stage1.ogg", 1.0f);
	


	return true;
}

// UnLoad assets
bool ModuleScene::CleanUp()
{
	LOG("Unloading space scene");

 	App->textures->Unload(background);
	App->player->Disable();
	App->collision->Disable();
	App->particles->Disable();
	
	return true;
}

// Update: draw background
update_status ModuleScene::Update()
{
	// Draw everything --------------------------------------
	App->floor->DrawBackground(background);
	App->floor->DrawStage(stage);
	App->floor->DrawFloor(floor);
	if (x++ >= 100000) {
		x2 = (RAND() % (SCREEN_WIDTH + 100)) - ((SCREEN_WIDTH / 2) + 50);
		//App->enemies->AddEnemy(*(App->enemies->GetById("rock1")), x2,150,MAX_Z);
		x2 = (RAND() % (SCREEN_WIDTH + 100)) - ((SCREEN_WIDTH / 2) + 50);
		//App->enemies->AddEnemy(*(App->enemies->GetById("tree1")), x2, 0, MAX_Z);
		x2 = (RAND() % (SCREEN_WIDTH + 100)) - ((SCREEN_WIDTH / 2) + 50);
		//App->enemies->AddEnemy(*(App->enemies->GetById("jelly1")), x2, 0, MAX_Z);
		//App->enemies->AddEnemy(*(App->enemies->GetById("jelly2")), x2, 0, MAX_Z);
		x2 = (RAND() % (SCREEN_WIDTH + 100)) - ((SCREEN_WIDTH / 2) + 50);
		//App->enemies->AddEnemy(*(App->enemies->GetById("drone2")), -150, 50, 0);
		//App->enemies->AddEnemy(*(App->enemies->GetById("drone3")), 150, 50, 0);
		x2 = (RAND() % (SCREEN_WIDTH + 100)) - ((SCREEN_WIDTH / 2) + 50);
		//App->enemies->AddEnemy(*(App->enemies->GetById("fly4")), -150, 340, MAX_Z);
		//App->enemies->AddEnemy(*(App->enemies->GetById("fly2")), 0, 340, MAX_Z);
		//App->enemies->AddEnemy(*(App->enemies->GetById("fly5")), 150, 340, MAX_Z);
		App->enemies->AddEnemy(*(App->enemies->GetById("dragon3_1")), 0, 200, 20);
		//App->enemies->AddEnemy(*(App->enemies->GetById("dragon3_2")), 0, 200, 20);
		x = 0;
	}
	App->renderer->Print(blue,-120,210,"Stage 3");
	return UPDATE_CONTINUE;
}

bool ModuleScene::LoadJson(string path) {
	json input;
	ifstream ifs(path);
	if (ifs.fail()) {
		cerr << "\nThe file Info.json could not be found in it's directory" << endl;
		return false;
	}
	ifs >> input;


	//Background
	string tempBPath = input["background"];
	backgroundPath = tempBPath;

	//Stage
	string tempSPath = input["stage"];
	stagePath = tempSPath;

	//Floor
	string tempFPath = input["floor"];
	floorPath = tempFPath;

	//Enemies
	for (unsigned int i = 0; i < input["enemies"].size(); ++i) {
		list<EnemyInstantiate> tempList;
		for (unsigned j = 0; j < input["enemies"][i].size(); ++j) {
			int x = input["enemies"][i][j]["x"];
			int y = input["enemies"][i][j]["y"];
			string id = input["enemies"][i][j]["id"];
			Enemy* enemy = App->enemies->GetById(id);
			EnemyInstantiate temp = { x,y,enemy };
			tempList.push_back(temp);
		}
		elements.push_back(tempList);
	}
	return true;
}
