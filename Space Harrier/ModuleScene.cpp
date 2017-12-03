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

const float ModuleScene::STARTING_DELAY = 2.0f;
const float ModuleScene::INTERVAL_DELAY = 0.1f;

ModuleScene::ModuleScene(bool active) : Module(active)
{
	x = 0;
	x2 = 100000;
	timeElapsed = 0;
}

ModuleScene::~ModuleScene()
{}

// Load assets
bool ModuleScene::Start()
{
	LOG("Loading space scene");

	LoadJson("assets/json/Stage3.json");
	
	background = App->textures->Load(backgroundPath.c_str());
	stage = App->textures->Load(stagePath.c_str());
	floor = App->textures->Load(floorPath.c_str());
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

update_status ModuleScene::Update()
{
	// Draw everything --------------------------------------
	App->floor->DrawBackground(background);
	App->floor->DrawStage(stage);
	App->floor->DrawFloor(floor);

	if (elements.size() > 0) {
		DelayList tempList = elements.front();
		if (timeElapsed >= STARTING_DELAY + INTERVAL_DELAY + tempList.delay) {
			for (list<EnemyInstantiate>::iterator it = tempList.list.begin(); it != tempList.list.end(); ++it) {
				EnemyInstantiate e = (*it);
				App->enemies->AddEnemy(*(e.enemy), e.x, e.y, e.z);
			}
			elements.pop_front();
			timeElapsed = STARTING_DELAY;
		}
		else
		{
			timeElapsed += App->time->GetDeltaTime();
		}
	}

	App->renderer->Print(blue, -120, 210, "Stage 3");
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
		DelayList tempList;
		tempList.delay= input["enemies"][i]["delay"];
		for (unsigned j = 0; j < input["enemies"][i]["list"].size(); ++j) {
			float x = input["enemies"][i]["list"][j]["x"];
			float y = input["enemies"][i]["list"][j]["y"];
			float z = input["enemies"][i]["list"][j]["z"];
			if (z > 0) {
				z = MAX_Z;
			}
			string id = input["enemies"][i]["list"][j]["id"];
			Enemy* enemy = App->enemies->GetById(id);
			EnemyInstantiate temp = { x,y,z,enemy };
			tempList.list.push_back(temp);
		}
		elements.push_back(tempList);
	}
	return true;
}
