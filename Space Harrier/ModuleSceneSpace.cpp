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
	z = 25;
	z2 = 25;
	x = 10;
	x2 = -100;
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
	 
	z -= 0.25;
	z2 -= 0.25;
	if (z <= 0) {
		z = 25;
		x = (RAND() % (SCREEN_WIDTH + 100)) - ((SCREEN_WIDTH / 2) + 50);
	}
	if (z2 <= 0) {
		z2 = 25;
		x2 = (RAND() % (SCREEN_WIDTH + 100)) - ((SCREEN_WIDTH / 2) + 50);
	}
	
	

	// Draw everything --------------------------------------
	App->renderer->Blit(background, 0, 0, nullptr, nullptr);
	SDL_Rect screenPoint = App->renderer->ToScreenPoint(x,0,z,&(tree.GetCurrentFrame()));
	App->renderer->Blit(trees,screenPoint.x,screenPoint.y,&(tree.GetCurrentFrame()),&screenPoint);
	SDL_Rect screenPoint2 = App->renderer->ToScreenPoint(x2, 0, z2, &(tree.GetCurrentFrame()));
	App->renderer->Blit(trees, screenPoint2.x, screenPoint2.y, &(tree.GetCurrentFrame()), &screenPoint2);

	return UPDATE_CONTINUE;
}