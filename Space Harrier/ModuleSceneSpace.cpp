#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "ModuleSceneSpace.h"
#include "ModuleEnemy.h"
#include "ModuleFont.h"

// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA

ModuleSceneSpace::ModuleSceneSpace(bool active) : Module(active)
{
	x = 1000.0f;
	x2 = 0.0f;
}

ModuleSceneSpace::~ModuleSceneSpace()
{}

// Load assets
bool ModuleSceneSpace::Start()
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
bool ModuleSceneSpace::CleanUp()
{
	LOG("Unloading space scene");

 	App->textures->Unload(background);
	App->player->Disable();
	App->collision->Disable();
	App->particles->Disable();
	
	return true;
}

// Update: draw background
update_status ModuleSceneSpace::Update()
{
	// Draw everything --------------------------------------
	App->renderer->DrawBackground(background);
	App->renderer->DrawStage(stage);
	App->renderer->DrawFloor(floor);
	if (x++ >= 100) {
		x2 = (RAND() % (SCREEN_WIDTH + 100)) - ((SCREEN_WIDTH / 2) + 50);
		App->enemies->AddEnemy(*(App->enemies->tree), x2,0,MAX_Z);
		x = 0;
	}
	App->renderer->Print(blue,-120,210,"Stage 3");
	return UPDATE_CONTINUE;
}