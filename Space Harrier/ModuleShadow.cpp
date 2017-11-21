#include "Application.h"
#include "ModuleShadow.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "SDL\include\SDL.h"

ModuleShadow::ModuleShadow() {
	shadowRect = {1,13,37,18};
}

ModuleShadow::~ModuleShadow() {
}

bool ModuleShadow::Start() {
	LOG("Loading shadow");
	shadow = App->textures->Load("assets/Shadow.png");

	return true;
}

bool ModuleShadow::CleanUp() {
	LOG("Unloading shadow");
	App->textures->Unload(shadow);

	return true;
}

void ModuleShadow::DrawShadow(float x, float y,float scale) {
	resizeStruct resizeInfo = {(int)(shadowRect.w*scale),(int)(shadowRect.h*scale) };
	App->renderer->AddToBlitBuffer(shadow,x,y,SHADOW_Z,&shadowRect,&resizeInfo);
}