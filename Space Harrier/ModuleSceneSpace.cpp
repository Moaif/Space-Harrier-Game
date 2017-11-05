#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "ModuleSceneSpace.h"

// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA

ModuleSceneSpace::ModuleSceneSpace(bool active) : Module(active)
{
	tree.frames.push_back({206,48,44,163});
	z = 0;
	sw = false;
}

ModuleSceneSpace::~ModuleSceneSpace()
{}

// Load assets
bool ModuleSceneSpace::Start()
{
	LOG("Loading space scene");
	
	background = App->textures->Load("assets/background.png");
	trees = App->textures->Load("assets/Arboles.png");

	App->player->Enable();
	App->particles->Enable();
	App->collision->Enable();

	App->audio->PlayMusic("assets/stage1.ogg", 1.0f);
	
	// TODO 15: create some colliders for the walls


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
	if (sw) {
		z--;
		if (z == 0) {
			sw = false;
		}
	}
	else
	{
		z++;
		if (z == 24) {
			sw = true;
		}
	}

	// Draw everything --------------------------------------
	App->renderer->Blit(background, 0, 0, 0, NULL);
	App->renderer->Blit(trees,100,0,z,&(tree.GetCurrentFrame()));
	
	return UPDATE_CONTINUE;
}