#include "Application.h"
#include "ModuleShadow.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "SDL\include\SDL.h"

ModuleShadow::ModuleShadow() {
	shadowRect = {6,16,37,18};
}

ModuleShadow::~ModuleShadow() {
}

bool ModuleShadow::Start() {
	LOG("Loading shadow");
	shadow = App->textures->Load("assets/Shadow.png");
	ASSERT(shadow != nullptr,AT("Failed on loading shadow texture"));

	return true;
}

bool ModuleShadow::CleanUp() {
	LOG("Unloading shadow");
	App->textures->Unload(shadow);

	return true;
}

void ModuleShadow::DrawShadow(const float& x,const float& y,const float& scale) {
	resizeStruct resizeInfo = {(int)(shadowRect.w*scale),(int)(shadowRect.h*scale) };
	App->renderer->AddToBlitBuffer(shadow,x,y,SHADOW_Z,&shadowRect,&resizeInfo);
}

SDL_Rect ModuleShadow::GetShadowSize()const {
	return shadowRect;
}