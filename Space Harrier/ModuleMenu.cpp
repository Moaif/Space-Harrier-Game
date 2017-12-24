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
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "ModuleEnemy.h"

const float ModuleMenu::TEXT_INTERVAL = 1.0f;

ModuleMenu::ModuleMenu(bool active) : Module(active)
{
	//Background
	background = {8,531,340,224};
	
	//Title
	title.frames.push_back({ 7,13,127,72 });
	title.frames.push_back({ 156,26,155,48 });
	title.frames.push_back({ 330,44,164,19 });
	title.frames.push_back({ 518,46,171,20 });
	title.frames.push_back({ 713,31,172,54 });
	title.frames.push_back({ 905,21,166,80 });
	title.frames.push_back({ 1105,11,163,92 });
	title.speed = 5.0f;

	finalTitle = { 1105,11,163,92 };

	//Eye
	eye.frames.push_back({8,141,35,17});
	eye.frames.push_back({ 51,141,35,17 });
	eye.frames.push_back({ 93,141,35,17 });
	eye.frames.push_back({ 135,141,35,17 });
	eye.frames.push_back({ 93,141,35,17 });
	eye.frames.push_back({ 51,141,35,17 });
	eye.frames.push_back({ 8,141,35,17 });
	eye.frames.push_back({ 178,141,35,17 });
	eye.frames.push_back({ 220,141,35,17 });
	eye.frames.push_back({ 263,141,35,17 });
	eye.frames.push_back({ 305,141,35,17 });
	eye.frames.push_back({ 347,141,35,17 });
	eye.frames.push_back({ 305,141,35,17 });
	eye.frames.push_back({ 263,141,35,17 });
	eye.speed = 3.0f;

	//Robot
	robot.frames.push_back({ 10,180,112,138 });
	robot.frames.push_back({ 130,180,112,138 });
	robot.frames.push_back({ 250,180,112,138 });
	robot.frames.push_back({ 370,180,112,138 });
	robot.frames.push_back({ 490,180,112,138 });
	robot.frames.push_back({ 610,180,112,138 });
	robot.frames.push_back({ 730,180,112,138 });
	robot.frames.push_back({ 850,180,112,138 });
	robot.frames.push_back({ 970,180,112,138 });
	robot.frames.push_back({ 1090,180,112,138 });
	robot.frames.push_back({ 1210,180,112,138 });
	robot.frames.push_back({ 1330,180,112,138 });
	robot.frames.push_back({ 1450,180,112,138 });
	robot.frames.push_back({ 10,330,112,138 });
	robot.frames.push_back({ 130,330,112,138 });
	robot.frames.push_back({ 250,330,112,138 });
	robot.frames.push_back({ 370,330,112,138 });
	robot.frames.push_back({ 490,330,112,138 });
	robot.frames.push_back({ 610,330,112,138 });
	robot.frames.push_back({ 730,330,112,138 });
	robot.frames.push_back({ 850,330,112,138 });
	robot.frames.push_back({ 970,330,112,138 });
	robot.frames.push_back({ 1090,330,112,138 });
	robot.frames.push_back({ 1210,330,112,138 });
	robot.frames.push_back({ 1330,330,112,138 });
	robot.frames.push_back({ 1450,330,112,138 });
	robot.speed = 5.0f;

	//Man
	man.frames.push_back({11,482,40,31 });
	man.frames.push_back({ 62,482,40,31 });
	man.frames.push_back({ 113,482,40,31 });
	man.frames.push_back({ 164,482,40,31 });
	man.speed = 2.0f;
}

ModuleMenu::~ModuleMenu()
{}

// Load assets
bool ModuleMenu::Start()
{
	LOG("Loading menu");
	
	content = App->textures->Load("assets/Menu.png");
	ASSERT(content != nullptr,AT("Menu failed at loading textures"));

	red = App->fonts->GetFont("Red",__FILE__,redFontLineReference = __LINE__);
	ASSERT(content != nullptr, AT("Menu failed at getting fonts"));

	App->Restart();//We restart all modules to start a new game
	
	if (!firstPlay) {
		App->player->Disable();
		App->ui->Disable();
		App->enemies->Disable();
		App->particles->Disable();
		App->collision->Disable();
	}
	firstPlay = false;

	titleMult = 4;
	textTimer = 0.0f;

	if (fx == 0) {
		fx = App->audio->LoadFx("assets/music/SFX/Coin.wav");
	}

	return true;
}

// UnLoad assets
bool ModuleMenu::CleanUp()
{
	LOG("Unloading menu");

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
	eye.Update();
	App->renderer->Blit(content, -82, 102, &(eye.GetCurrentFrame()), nullptr);

	//Robot
	robot.Update();
	App->renderer->Blit(content, 97,11,&(robot.GetCurrentFrame()),nullptr);

	//Man
	man.Update();
	App->renderer->Blit(content,101,146,&(man.GetCurrentFrame()),nullptr);

	//Title
	if (titleMult > 0) {
		title.Update();
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
			App->fade->FadeToBlack((Module*)App->scene, this);
			App->audio->PlayFx(fx);
		}
	}

	return UPDATE_CONTINUE;
}