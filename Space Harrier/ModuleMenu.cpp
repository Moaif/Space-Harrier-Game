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
	background = {1943,57,320,224};

	man.frames.push_back({ 17, 5, 51, 35 });
	man.frames.push_back({ 69, 5, 51, 35 });
	man.frames.push_back({ 17, 40, 51, 35 });
	man.frames.push_back({ 69, 5, 51, 35 });
	man.speed = 1.5f;

	eye.frames.push_back({ 17, 82, 31, 19 });
	eye.frames.push_back({ 50, 82, 31, 19 });
	eye.frames.push_back({ 84, 82, 31, 19 });
	eye.frames.push_back({ 50, 82, 31, 19 });
	eye.frames.push_back({ 17, 82, 31, 19 });
	eye.frames.push_back({ 17, 103, 31, 19 });
	eye.frames.push_back({ 50, 103, 31, 19 });
	eye.frames.push_back({ 84, 103, 31, 19 });
	eye.frames.push_back({ 50, 103, 31, 19 });
	eye.frames.push_back({ 17, 103, 31, 19 });
	eye.frames.push_back({ 17, 82, 31, 19 });
	eye.frames.push_back({ 17, 123, 31, 19 });
	eye.frames.push_back({ 50, 123, 31, 19 });
	eye.frames.push_back({ 84, 123, 31, 19 });
	eye.frames.push_back({ 50, 123, 31, 19 });
	eye.frames.push_back({ 17, 123, 31, 19 });
	eye.speed = 4.0f;

	title.frames.push_back({ 129, 5, 158, 92 });
	title.frames.push_back({ 129, 102, 158, 92 });
	title.frames.push_back({ 129, 200, 158, 92 });
	title.frames.push_back({ 295, 5, 158, 92 });
	title.frames.push_back({ 295, 102, 158, 92 });
	title.frames.push_back({ 295, 200, 158, 92 });
	title.frames.push_back({ 459, 5, 158, 92 });
	title.frames.push_back({ 459, 102, 158, 92 });
	title.frames.push_back({ 459, 200, 158, 92 });
	title.frames.push_back({ 623, 5, 158, 92 });
	title.frames.push_back({ 623, 102, 158, 92 });
	title.frames.push_back({ 623, 200, 158, 92 });
	title.frames.push_back({ 786, 5, 158, 92 });
	title.frames.push_back({ 786, 102, 158, 92 });
	title.frames.push_back({ 786, 200, 158, 92 });
	title.frames.push_back({ 948, 5, 158, 92 });
	title.frames.push_back({ 948, 102, 158, 92 });
	title.frames.push_back({ 948, 200, 158, 92 });
	title.frames.push_back({ 1110, 5, 158, 92 });
	title.frames.push_back({ 1110, 102, 158, 92 });
	title.frames.push_back({ 1110, 200, 158, 92 });
	title.frames.push_back({ 1275, 5, 158, 92 });
	title.frames.push_back({ 1275, 102, 158, 92 });
	title.frames.push_back({ 1275, 200, 158, 92 });
	title.frames.push_back({ 1439, 5, 158, 92 });
	title.frames.push_back({ 1439, 102, 158, 92 });
	title.frames.push_back({ 1439, 200, 158, 92 });
	title.frames.push_back({ 1603, 5, 158, 92 });
	title.frames.push_back({ 1603, 102, 158, 92 });
	title.frames.push_back({ 1603, 200, 158, 92 });
	title.frames.push_back({ 1766, 5, 158, 92 });
	title.frames.push_back({ 1766, 102, 158, 92 });
	title.loop = false;
	title.speed = 4.0f;
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
	App->renderer->Blit(content, -79, 102, &(eye.GetCurrentFrame()), nullptr);

	//Man
	man.Update();
	App->renderer->Blit(content,100,151,&(man.GetCurrentFrame()),nullptr);

	// Title
	title.Update();
	App->renderer->Blit(content, 0, 130, &(title.GetCurrentFrame()), nullptr);

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