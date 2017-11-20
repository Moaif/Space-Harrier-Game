#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleFadeToBlack.h"
#include "ModuleSceneIntro.h"
#include "ModulePlayer.h"
#include "ModuleFont.h"

// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA

ModuleSceneIntro::ModuleSceneIntro(bool active) : Module(active)
{
	robot.frames.push_back({ 2,0,280,325 });
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading space intro");
	
	background = App->textures->Load("assets/intro.png");

	App->audio->PlayMusic("assets/intro.ogg", 1.0f);
	if(fx == 0)
		fx = App->audio->LoadFx("assets/starting.wav");

	red = App->fonts->GetFont("Red",__FILE__,__LINE__);
	blue = App->fonts->GetFont("Blue", __FILE__, __LINE__);
	green = App->fonts->GetFont("Green", __FILE__, __LINE__);
	yellow = App->fonts->GetFont("Yellow", __FILE__, __LINE__);

	return true;
}

// UnLoad assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading space scene");

	App->textures->Unload(background);
	
	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	resizeStruct resizeInfo {70,100 };
	App->renderer->Blit(background, 100, 0, &(robot.GetCurrentFrame()), &resizeInfo );
	App->renderer->Print(red,-40,0,"Esto es el menu");
	App->renderer->Print(blue, -40, 40, "Esto es el menu");
	App->renderer->Print(green, -40, 80, "Esto es el menu");
	App->renderer->Print(yellow, -40, 120, "Esto es el menu");


	if(App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		if (App->fade->isFading() == false && App->player->GetLives() > 0) {
			App->fade->FadeToBlack((Module*)App->scene_space, this);
			App->audio->PlayFx(fx);
		}
		else
		{
			App->player->SetLives(3);
		}
	}

	return UPDATE_CONTINUE;
}