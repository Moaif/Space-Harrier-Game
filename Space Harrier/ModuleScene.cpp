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
#include "ModuleUI.h"
#include "json.hpp"
#include <fstream>

using json = nlohmann::json;

const float ModuleScene::STARTING_DELAY = 2.0f;
const float ModuleScene::INTERVAL_DELAY = 0.1f;

ModuleScene::ModuleScene(bool active) : Module(active)
{
	timeElapsed = 0;
	currentStage = 2;//TODO: if introduced new stages, change
}

ModuleScene::~ModuleScene()
{}

// Load assets
bool ModuleScene::Start()
{
	LOG("Loading space scene");

	currentStage++;//TODO: Finish loadJson with currenStage when more stages are added
	LoadJson("assets/json/Stage3.json");
	
	background = App->textures->Load(backgroundPath.c_str());
	stage = App->textures->Load(stagePath.c_str());
	floor = App->textures->Load(floorPath.c_str());

	App->player->Enable();
	App->particles->Enable();
	App->collision->Enable();
	App->ui->Enable();
	App->playing = true;

	App->audio->PlayMusic("assets/music/Theme.wav", 1.0f);
	


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
	App->ui->Disable();
	
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

			//Only BOSS to be instantiated
			if (elements.size() == 1) {
				string path = "assets/music/Stage" + to_string(currentStage) + "Boss.wav";
				App->audio->PlayMusic(path.c_str(), 1.0f);
			}

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
	//Stage name
	string tempName = input["name"];
	stageName = tempName;

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
