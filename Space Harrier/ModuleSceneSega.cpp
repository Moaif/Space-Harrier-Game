#include "Application.h"
#include "ModuleSceneSega.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleRender.h"
#include "ModuleFadeToBlack.h"
#include "SDL\include\SDL.h"

const float ModuleSceneSega::OFFSET = 2.0f;
const float ModuleSceneSega::INTERVAL = 0.025f;

ModuleSceneSega::ModuleSceneSega(bool active):Module(active) {
}

ModuleSceneSega::~ModuleSceneSega() {
}

bool ModuleSceneSega::Start() {
	LOG("Loading sega intro");

	background = App->textures->Load("assets/SegaLogo.png");
	ASSERT(background != nullptr,AT("Failed on loading SegaLogo"));
	
	SDL_QueryTexture(background, NULL, NULL, &totalRect.w, &totalRect.h);

	image = { totalRect.w/2.0f, totalRect.h/2.0f, 0, 0};
	timer = 0.0f;

	fx = App->audio->LoadFx("assets/music/SFX/Sega.wav");
	App->audio->PlayFx(fx);

	return true;
}

bool ModuleSceneSega::CleanUp() {
	LOG("Unloading sega intro");

	App->textures->Unload(background);

	return true;
}

update_status ModuleSceneSega::Update() {
	float coefX = SCREEN_WIDTH/image.w;
	float coefY = SCREEN_HEIGHT/image.h;
	resizeStruct size = {(int)(coefX*image.w),(int)(coefY*image.h)};
	SDL_Rect rect = { (int)image.x,(int)image.y,(int)image.w,(int)image.h };
	App->renderer->Blit(background, 0, 0, &rect, &size);
	

	timer += App->time->GetDeltaTime();
	if (image.w < SCREEN_WIDTH && image.h < SCREEN_HEIGHT) {
		if (timer > INTERVAL) {
			float ratio = (float)totalRect.w / totalRect.h;
			float x = image.x - OFFSET*ratio;
			float y = image.y - OFFSET;
			float w = image.w + OFFSET*ratio*2;
			float h = image.h + OFFSET*2;
			image = { x,y,w,h };
			timer = 0;
		}
	}

	if (timer > INTERVAL*5) {
		if (App->fade->isFading() == false) {
			App->fade->FadeToBlack((Module*)App->menu, this);
		}
	}
	

	return UPDATE_CONTINUE;
}