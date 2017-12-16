#include "Application.h"
#include "ModuleShadow.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "SDL\include\SDL.h"

ModuleShadow::ModuleShadow() {
	shadowRect = {2,14,45,22};
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

void ModuleShadow::DrawShadow(const float& x,const float& y,const float& scale) {
	resizeStruct resizeInfo = {(int)(shadowRect.w*scale),(int)(shadowRect.h*scale) };
	App->renderer->AddToBlitBuffer(shadow,x,y,SHADOW_Z,&shadowRect,&resizeInfo);
}