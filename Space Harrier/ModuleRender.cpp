#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModulePlayer.h"
#include "SDL/include/SDL.h"
#include <iostream>
#include "Font.h"
#include <math.h>
#ifdef _DEBUG
#include "Brofiler.h"
#endif // _DEBUG

using namespace std;



ModuleRender::ModuleRender()
{
	nearClippingPlane = 2;

}

// Destructor
ModuleRender::~ModuleRender()
{}

// Called before render is available
bool ModuleRender::Init()
{
	LOG("Creating Renderer context");
	bool ret = true;
	Uint32 flags = 0;

	if(VSYNC == true)
	{
		flags |= SDL_RENDERER_PRESENTVSYNC;
	}

	renderer = SDL_CreateRenderer(App->window->window, -1, flags);
	
	if(renderer == nullptr)
	{
		LOG("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

update_status ModuleRender::PreUpdate()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleRender::Update()
{
	while (!blitQueue.empty())
	{
		BlitStruct temp = blitQueue.top();
		if (temp.sectionNull && temp.blitSectionNull) {
			Blit(temp.texture, temp.x, temp.y, nullptr, nullptr);
		}
		else if (temp.sectionNull) {
			Blit(temp.texture, temp.x, temp.y, nullptr, &temp.blitSection);
		}
		else if (temp.blitSectionNull)
		{
			Blit(temp.texture, temp.x, temp.y, &temp.section, nullptr);
		}
		else {
			Blit(temp.texture, temp.x, temp.y, &temp.section, &temp.blitSection);
		}
		blitQueue.pop();
	}
	return UPDATE_CONTINUE;
}

update_status ModuleRender::PostUpdate()
{
	#ifdef _DEBUG
		BROFILER_CATEGORY("Render", Profiler::Color::Orchid)
	#endif // _DEBUG
	SDL_RenderPresent(renderer);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRender::CleanUp()
{
	LOG("Destroying renderer");

	//Destroy window
	if(renderer != nullptr)
	{
		SDL_DestroyRenderer(renderer);
	}

	return true;
}

void ModuleRender::AddToBlitBuffer(SDL_Texture* texture, float x, float y,float z, SDL_Rect* section, resizeStruct* resizeInfo) {
	SDL_Rect empty = { 0,0,0,0 };
	resizeStruct noResize= { 0,0 };
	BlitStruct temp;
	if (section == nullptr && resizeInfo == nullptr) {
		temp = { texture,x,y,z,empty,noResize,true,true };
	}
	else if (section == nullptr) {
		temp = { texture,x,y,z,empty,*resizeInfo,true,false };
	}

	else if (resizeInfo == nullptr) {
		temp = { texture,x,y,z,*section,noResize,false,true };
	}
	else {
		temp = { texture,x,y,z,*section,*resizeInfo,false,false };
	}
	blitQueue.push(temp);
}

// Blit to screen
bool ModuleRender::Blit(SDL_Texture* texture, float x, float y, SDL_Rect* section, resizeStruct* resizeInfo)
{
	bool ret = true;
	SDL_Rect rect;

	if(resizeInfo != NULL)
	{
		rect.w = resizeInfo->w;
		rect.h = resizeInfo->h;
	}
	else if (section !=NULL) {
		rect.w = section->w;
		rect.h = section->h;
	}
	else
	{
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	}

	//Center (0,0) is in the mid-down of the window
	x = x + (SCREEN_WIDTH / 2);
	y = SCREEN_HEIGHT - y;

	//Now we calculate the left-top point where the image should start
	x = x - (rect.w / 2);
	y = y - rect.h ;

	rect.x =(int)( x * SCREEN_SIZE);
	rect.y =(int)( y * SCREEN_SIZE);

	rect.w *= SCREEN_SIZE;
	rect.h *= SCREEN_SIZE;

	if(SDL_RenderCopy(renderer, texture, section, &rect) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool ModuleRender::Print(const Font* font, float x, float y, string mesage, float fontSize) {
	bool ret = true;
	int xSize = font->GetXSize();
	int ySize = font->GetYSize();

	if (fontSize < 0) {
		fontSize = 0;
	}

	SDL_Surface* tempSurface = font->GetImage();
	SDL_Surface* surfaceFinal = SDL_CreateRGBSurface(0, mesage.length() * xSize, ySize, 32, 0, 0, 0, 0);
	SDL_FillRect(surfaceFinal, NULL, 0xFF00FF);
	SDL_SetColorKey(surfaceFinal, SDL_TRUE, SDL_MapRGB(surfaceFinal->format, 255, 0, 255));

	SDL_Rect srcrect;
	srcrect.h = ySize;
	srcrect.w = xSize;
	SDL_Rect dstrect;
	dstrect.h = ySize;
	dstrect.w = xSize;
	if (tempSurface == nullptr)
	{
		printf("Unable to load image %s! SDL Error: %s\n", "Font.bmp", SDL_GetError());
		ret = false;
	}
	for (unsigned int i = 0; i < mesage.size(); ++i) {
		int offset = font->GetCharOffset(mesage[i]);
		srcrect.x = offset*xSize;
		srcrect.y = 0;
		dstrect.x = i*xSize;
		dstrect.y = 0;
		SDL_BlitSurface(tempSurface, &srcrect, surfaceFinal, &dstrect);
	}
	SDL_Texture* tempTexture = SDL_CreateTextureFromSurface(renderer, surfaceFinal);
	if (tempTexture == nullptr) {
		printf("Unable to create texture from surface SDL Error: %s\n", SDL_GetError());
		ret = false;
	}

	SDL_Rect rect;
	SDL_QueryTexture(tempTexture, NULL, NULL, &rect.w, &rect.h);
	resizeStruct size = { rect.w*fontSize,rect.h*fontSize };

	AddToBlitBuffer(tempTexture, x, y, FONTS_Z, nullptr, &size);
	SDL_FreeSurface(surfaceFinal);

	return ret;
}

bool ModuleRender::DirectPrint(const Font* font, float x, float y, string mesage, float fontSize) {
	bool ret = true;
	int xSize = font->GetXSize();
	int ySize = font->GetYSize();

	if (fontSize < 0) {
		fontSize = 0;
	}

	SDL_Surface* tempSurface = font->GetImage();
	SDL_Surface* surfaceFinal = SDL_CreateRGBSurface(0, mesage.length() * xSize, ySize, 32, 0, 0, 0, 0);
	SDL_FillRect(surfaceFinal, NULL, 0xFF00FF);
	SDL_SetColorKey(surfaceFinal, SDL_TRUE, SDL_MapRGB(surfaceFinal->format, 255, 0, 255));

	SDL_Rect srcrect;
	srcrect.h = ySize;
	srcrect.w = xSize;
	SDL_Rect dstrect;
	dstrect.h = ySize;
	dstrect.w = xSize;
	if (tempSurface == nullptr)
	{
		printf("Unable to load image %s! SDL Error: %s\n", "Font.bmp", SDL_GetError());
		ret = false;
	}
	for (unsigned int i = 0; i < mesage.size(); ++i) {
		int offset = font->GetCharOffset(mesage[i]);
		srcrect.x = offset*xSize;
		srcrect.y = 0;
		dstrect.x = i*xSize;
		dstrect.y = 0;
		SDL_BlitSurface(tempSurface, &srcrect, surfaceFinal, &dstrect);
	}
	SDL_Texture* tempTexture = SDL_CreateTextureFromSurface(renderer, surfaceFinal);
	if (tempTexture == nullptr) {
		printf("Unable to create texture from surface SDL Error: %s\n", SDL_GetError());
		ret = false;
	}

	SDL_Rect rect;
	SDL_QueryTexture(tempTexture, NULL, NULL, &rect.w, &rect.h);
	resizeStruct size = { rect.w*fontSize,rect.h*fontSize };

	Blit(tempTexture, x, y, nullptr, &size);
	SDL_FreeSurface(surfaceFinal);

	return ret;
}

bool ModuleRender::DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	bool ret = true;
	SDL_Rect tempRect;

	//Center (0,0) is in the mid-down of the window
	tempRect.x = rect.x + (SCREEN_WIDTH / 2);
	tempRect.y = SCREEN_HEIGHT - rect.y;

	//Now we calculate the left-top point where the image should start
	tempRect.x = tempRect.x - (rect.w / 2);
	tempRect.y = tempRect.y - rect.h;
	tempRect.w = rect.w;
	tempRect.h = rect.h;


	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	tempRect.x = (int)(tempRect.x * SCREEN_SIZE);
	tempRect.y = (int)(tempRect.y * SCREEN_SIZE);
	tempRect.w *= SCREEN_SIZE;
	tempRect.h *= SCREEN_SIZE;
	

	if (SDL_RenderFillRect(renderer, &tempRect) != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool ModuleRender::DrawQuads(const SDL_Rect rects[], int count, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	bool ret = true;

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	if (SDL_RenderFillRects(renderer, rects, count) != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRects error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}
