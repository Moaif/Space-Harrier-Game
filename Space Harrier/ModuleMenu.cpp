#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleFadeToBlack.h"
#include "ModuleMenu.h"
#include "ModulePlayer.h"
#include "ModuleFont.h"
#include "ModuleUI.h"

const float ModuleMenu::TEXT_INTERVAL = 1.0f;

ModuleMenu::ModuleMenu(bool active) : Module(active)
{
	//Background
	background = {8,528,340,224};
	
	//Title
	title.frames.push_back({ 0,10,140,90 });
	title.frames.push_back({ 150,20,170,60 });
	title.frames.push_back({ 330,30,170,40 });
	title.frames.push_back({ 510,40,190,30 });
	title.frames.push_back({ 710,20,180,70 });
	title.frames.push_back({ 1100,10,180,100 });

	finalTitle = {1420,10,170,100};

	titleMult = 4;

	//Eye
	eye.frames.push_back({8,140,34,18});
	eye.frames.push_back({ 50,140,34,18 });
	eye.frames.push_back({ 92,140,34,18 });
	eye.frames.push_back({ 134,140,34,18 });
	eye.frames.push_back({ 92,140,34,18 });
	eye.frames.push_back({ 50,140,34,18 });
	eye.frames.push_back({ 8,140,34,18 });
	eye.frames.push_back({ 178,140,34,18 });
	eye.frames.push_back({ 220,140,34,18 });
	eye.frames.push_back({ 263,140,34,18 });
	eye.frames.push_back({ 305,140,34,18 });
	eye.frames.push_back({ 347,140,34,18 });
	eye.frames.push_back({ 305,140,34,18 });
	eye.frames.push_back({ 263,140,34,18 });

	//Robot
	robot.frames.push_back({ 8,176,113,137 });
	robot.frames.push_back({ 126,176,113,137 });
	robot.frames.push_back({ 245,176,113,137 });
	robot.frames.push_back({ 364,176,113,137 });
	robot.frames.push_back({ 483,176,113,137 });
	robot.frames.push_back({ 602,176,113,137 });
	robot.frames.push_back({ 721,176,113,137 });
	robot.frames.push_back({ 840,176,113,137 });
	robot.frames.push_back({ 959,176,113,137 });
	robot.frames.push_back({ 1078,176,113,137 });
	robot.frames.push_back({ 1197,176,113,137 });
	robot.frames.push_back({ 1315,176,113,137 });
	robot.frames.push_back({ 1434,176,113,137 });
	robot.frames.push_back({ 8,325,113,137 });
	robot.frames.push_back({ 126,325,113,137 });
	robot.frames.push_back({ 245,325,113,137 });
	robot.frames.push_back({ 364,325,113,137 });
	robot.frames.push_back({ 483,325,113,137 });
	robot.frames.push_back({ 602,325,113,137 });
	robot.frames.push_back({ 721,325,113,137 });
	robot.frames.push_back({ 840,325,113,137 });
	robot.frames.push_back({ 959,325,113,137 });
	robot.frames.push_back({ 1078,325,113,137 });
	robot.frames.push_back({ 1197,325,113,137 });
	robot.frames.push_back({ 1315,325,113,137 });
	robot.frames.push_back({ 1434,325,113,137 });

	//Man
	man.frames.push_back({10,480,50,30});
	man.frames.push_back({ 60,480,50,30 });
	man.frames.push_back({ 110,480,50,30 });
	man.frames.push_back({ 160,480,50,30 });
}

ModuleMenu::~ModuleMenu()
{}

// Load assets
bool ModuleMenu::Start()
{
	LOG("Loading space intro");
	
	content = App->textures->Load("assets/Menu.png");
	red = App->fonts->GetFont("Red",__FILE__,redFontLineReference = __LINE__);
	textTimer = 0.0f;

	if(fx == 0)
		fx = App->audio->LoadFx("assets/music/SFX/Coin.wav");

	return true;
}

// UnLoad assets
bool ModuleMenu::CleanUp()
{
	LOG("Unloading space scene");

	App->textures->Unload(content);
	App->fonts->FreeFont(&red,__FILE__,redFontLineReference);
	
	return true;
}

// Update: draw background
update_status ModuleMenu::Update()
{
	//Background
	App->renderer->Blit(content, 0, 0, &background, nullptr );	

	//Eye
	App->renderer->Blit(content, -82, 102, &(eye.GetCurrentFrame()), nullptr);

	//Robot
	App->renderer->Blit(content, 97,12,&(robot.GetCurrentFrame()),nullptr);

	//Man
	App->renderer->Blit(content,105,146,&(man.GetCurrentFrame()),nullptr);

	//Title
	if (titleMult > 0) {
		resizeStruct size = { title.GetCurrentFrame().w / titleMult, title.GetCurrentFrame().h / titleMult };
		App->renderer->Blit(content, 0, 120, &(title.GetCurrentFrame()), &size);

		if (title.Finished()) {
			title.Reset();
			--titleMult;
		}
	}
	else
	{
		App->renderer->Blit(content, 0, 120, &(finalTitle), nullptr);
	}

	//Print "press start..."
	textTimer += App->time->GetDeltaTime();
	if (textTimer > TEXT_INTERVAL) {
		App->renderer->DirectPrint(red, 0, 60, "Press ENTER to continue", 0.75);
		if (textTimer > TEXT_INTERVAL * 2) {
			textTimer = 0.0f;
		}
	}

	if(App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		if (App->fade->isFading() == false) {
			App->player->SetLives(3);
			App->fade->FadeToBlack((Module*)App->scene, this);
			App->audio->PlayFx(fx);
		}
	}

	return UPDATE_CONTINUE;
}