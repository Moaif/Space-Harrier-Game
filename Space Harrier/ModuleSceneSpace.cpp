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
	z = 100;
	z2 = 100;
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
	floor = App->textures->Load("assets/Floor.png");

	App->player->Enable();
	App->particles->Enable();
	App->collision->Enable();
	App->playing = true;

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
	float speed = 0.1386;
	z -=(speed + speed * (z/MAX_Z)*SCREEN_SIZE);
	z2 -= (speed + speed * (z2 / MAX_Z)*SCREEN_SIZE);
	if (z <= -3) {
		z = 100;
		x = (RAND() % (SCREEN_WIDTH + 100)) - ((SCREEN_WIDTH / 2) + 50);
	}
	if (z2 <= -3) {
		z2 = 100;
		x2 = (RAND() % (SCREEN_WIDTH + 100)) - ((SCREEN_WIDTH / 2) + 50);
	}
	
	

	// Draw everything --------------------------------------
	App->renderer->Blit(background, 0, 0, nullptr, nullptr);
	SDL_Rect temp = {0,0,SCREEN_WIDTH,App->renderer->horizon.y};
	App->renderer->Blit(floor,0,0,nullptr,&temp);
	App->renderer->DrawAlphaLines();
	SDL_Rect screenPoint = App->renderer->ToScreenPoint(x,0,z,&(tree.GetCurrentFrame()));
	App->renderer->AddToBlitBuffer(trees,screenPoint.x,screenPoint.y,z,&(tree.GetCurrentFrame()),&screenPoint);
	SDL_Rect screenPoint2 = App->renderer->ToScreenPoint(x2, 0, z2, &(tree.GetCurrentFrame()));
	App->renderer->AddToBlitBuffer(trees, screenPoint2.x, screenPoint2.y,z2, &(tree.GetCurrentFrame()), &screenPoint2);

	return UPDATE_CONTINUE;
}