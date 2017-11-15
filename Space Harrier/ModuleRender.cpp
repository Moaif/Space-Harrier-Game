#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModulePlayer.h"
#include "SDL/include/SDL.h"
#include <iostream>
#include "Font.h"

using namespace std;


ModuleRender::ModuleRender()
{
	horizon = { 0,HORIZON_Y_MIN };
	alphaLineDistanceStart = ALPHA_DISTANCE_MIN;
	alphaLineSizeStart = ALPHA_SIZE_MIN;
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

void ModuleRender::AddToBlitBuffer(SDL_Texture* texture, int x, int y,int z, SDL_Rect* section, SDL_Rect* blitSection) {
	SDL_Rect empty = { 0,0,0,0 };
	BlitStruct temp;
	if (section == nullptr && blitSection == nullptr) {
		temp = { texture,x,y,z,empty,empty,true,true };
	}
	else if (section == nullptr) {
		temp = { texture,x,y,z,empty,*blitSection,true,false };
	}

	else if (blitSection == nullptr) {
		temp = { texture,x,y,z,*section,empty,false,true };
	}
	else {
		temp = { texture,x,y,z,*section,*blitSection,false,false };
	}
	blitQueue.push(temp);
}

// Blit to screen
bool ModuleRender::Blit(SDL_Texture* texture, int x, int y, SDL_Rect* section, SDL_Rect* blitSection)
{
	bool ret = true;
	SDL_Rect rect;

	if(blitSection != NULL)
	{
		rect.w = blitSection->w;
		rect.h = blitSection->h;
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

	rect.x = x * SCREEN_SIZE;
	rect.y = y * SCREEN_SIZE;

	rect.w *= SCREEN_SIZE;
	rect.h *= SCREEN_SIZE;

	if(SDL_RenderCopy(renderer, texture, section, &rect) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool ModuleRender::Print(const Font* font, int x, int y, string mesage) {
	bool ret = true;
	int xSize = font->GetXSize();
	int ySize = font->GetYSize();

	SDL_Surface* tempSurface = font->GetImage();
	SDL_Surface* surfaceFinal = SDL_CreateRGBSurface(0, mesage.length() * xSize , ySize, 32, 0, 0, 0, 0);
	SDL_SetColorKey(surfaceFinal, SDL_TRUE, SDL_MapRGB(surfaceFinal->format, 0, 0, 0));

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
	SDL_Texture* tempTexture= SDL_CreateTextureFromSurface(renderer,surfaceFinal);
	if (tempTexture == nullptr) {
		printf("Unable to create texture from surface SDL Error: %s\n", SDL_GetError());
		ret = false;
	}
	AddToBlitBuffer(tempTexture, x, y,FONTS_Z, nullptr, nullptr);

	return ret;
}

bool ModuleRender::DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera)
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

bool ModuleRender::DrawBackground(SDL_Texture* texture) {
	bool ret = true;
	SDL_Rect rect;
	int w, h;

	SDL_QueryTexture(texture, NULL, NULL, &w, &h);

	rect.h = h;
	rect.w = SCREEN_WIDTH;
	rect.x = (int)backgroundOffset;
	rect.y = 0;

	if ((backgroundOffset + SCREEN_WIDTH) > w) {
		SDL_Rect rect2 = { 0,0,(int)((backgroundOffset + SCREEN_WIDTH) - w),h };
		rect.w = SCREEN_WIDTH - rect2.w+1;//With round, somethimes it leaves 1 pixel without drawing, thats why we add +1
		if (!Blit(texture, (-(SCREEN_WIDTH / 2)) + (rect.w/ 2), horizon.y, &rect, nullptr)) {
			return false;
		}
		ret = Blit(texture, (SCREEN_WIDTH / 2) - (rect2.w / 2), horizon.y, &rect2, nullptr);
	}
	else
	{
		ret = Blit(texture, 0, horizon.y, &rect, nullptr);
	}

	backgroundOffset += backgroundSpeed;
	if (backgroundOffset >= w) {
		backgroundOffset = 0;
	}
	if (backgroundOffset < 0) {
		backgroundOffset = (float)w;
	}

	return ret;
}

bool ModuleRender::DrawStage(SDL_Texture* texture) {
	bool ret = true;
	SDL_Rect rect;
	int w, h;

	SDL_QueryTexture(texture, NULL, NULL, &w, &h);

	rect.h = h;
	rect.w = SCREEN_WIDTH;
	rect.x = (int)stageOffset;
	rect.y = 0;

	if ((stageOffset + SCREEN_WIDTH) > w) {
		SDL_Rect rect2 = { 0,0,(int)((stageOffset + SCREEN_WIDTH) - w),h };
		rect.w = SCREEN_WIDTH - rect2.w;
		if (!Blit(texture, (-(SCREEN_WIDTH / 2)) + (rect.w / 2), horizon.y, &rect, nullptr)) {
			return false;
		}
		ret=Blit(texture,(SCREEN_WIDTH/2)-(rect2.w/2),horizon.y,&rect2,nullptr);
	}
	else
	{
		ret= Blit(texture, 0, horizon.y, &rect, nullptr);
	}

	stageOffset += stageSpeed;
	if (stageOffset >= w) {
		stageOffset = 0;
	}
	if (stageOffset < 0) {
		stageOffset = (float)w;
	}

	return ret;
}

bool ModuleRender::DrawFloor(SDL_Texture* texture)
{
	bool ret = true;
	SDL_Rect rect;
	int textW, textH;

	SDL_QueryTexture(texture, NULL, NULL, &textW, &textH);

	rect.w = SCREEN_WIDTH;
	rect.h = horizon.y;

	//Set (0,0) on the bottom-middle screen.
	int pX = (SCREEN_WIDTH / 2) - (rect.w / 2);
	int pY = SCREEN_HEIGHT - rect.h;

	rect.x = pX * SCREEN_SIZE;
	rect.y = pY * SCREEN_SIZE;
	rect.w *= SCREEN_SIZE;
	rect.h *= SCREEN_SIZE;

	float maxAditionalPixelsX = ((textW - SCREEN_WIDTH) / 2.0f);

	if (increasePixelIteration >= 120.0f || increasePixelIteration <= -120.0f)
	{
		increasePixelIteration = 2.0f;
	}
	increasePixelIteration += App->player->speed;

	float pixelsPerRow = (float)textH / rect.h;
	float pixelsPerRowOffset = 0.0f;

	SDL_Rect textureLine = { (int)maxAditionalPixelsX, 0, (int)(textW-maxAditionalPixelsX*2), 1 };
	rect.h = 1;

	int originalRectX = textureLine.x;
	float deviation = 0.0f;

	for (int i = 0; i <= (horizon.y*SCREEN_SIZE); ++i)
	{
		deviation = (((float)i / ((float)horizon.y*(float)SCREEN_SIZE))*increasePixelIteration);
		textureLine.x = originalRectX + (int)deviation;
		if (SDL_RenderCopy(renderer, texture, &textureLine, &rect) !=0 ) {
			LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
			ret = false;
		}
		pixelsPerRowOffset += pixelsPerRow;
		textureLine.y = (int)pixelsPerRowOffset;
		rect.y += 1;
	}

	DrawAlphaLines();

	return ret;
}

void ModuleRender::DrawAlphaLines()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100);
	alphaLineDistance = alphaLineDistanceStart;
	alphaLineSize = alphaLineSizeStart;
	float offsetDif = 0;
	float distDif = alphaLineDistanceStart;
	float normalIteration = alphaLineIteration*((alphaLineDistanceStart + alphaLineSizeStart) / ySpeed);
	int tempIteration = (int)normalIteration % (int)(alphaLineDistanceStart * 2);
	float coef = tempIteration / alphaLineDistance*2;
	if (coef >= 1) {
		alphaLineIteration = 0;
	}


	while ((alphaLineDistance+alphaLineSizeStart) <= (horizon.y*SCREEN_SIZE))
	{
		const SDL_Rect test = { 0,(int)( SCREEN_HEIGHT*SCREEN_SIZE - ((alphaLineDistance+alphaLineSizeStart)-(coef*(distDif+alphaLineSize)))), SCREEN_WIDTH*SCREEN_SIZE, (int)(alphaLineSize+(offsetDif*coef)) };
		SDL_RenderFillRect(renderer, &test);

		offsetDif = alphaLineSize / 2.8f;
		alphaLineSize -= offsetDif;
		if (alphaLineSize <= 1) {
			alphaLineSize = 1;
		}
		distDif = distDif*0.65f;
		if (distDif <=1) {
			distDif = 1;
		}
		alphaLineDistance += distDif+alphaLineSize;
	}

	alphaLineIteration = (alphaLineIteration + 2);
}

SDL_Rect ModuleRender::ToScreenPoint(float x,float y,float z,SDL_Rect* section) {

	SDL_Rect rect;

	float scale = DepthScale(z);
	float inversescale = 1.0f - scale;

	float temp = section->h - (scale*section->h);
	rect.w = (int)(scale*section->w);
	rect.h = (int)(scale*section->h);

	rect.x =(int) ((x * scale) + (horizon.x * inversescale));
	rect.y =(int) ((y * scale) + (horizon.y * inversescale));

	return rect;
}

SDL_Rect ModuleRender::ToScreenPointBasic(float x, float y, float z, SDL_Rect* section) {
	SDL_Rect rect;

	float scale = DepthScale(z);

	rect.w = section->w*scale;
	rect.h = section->h*scale;
	LOG("h: %d hS: %d", section->h,rect.h);
	float wDiff = section->w - rect.w;
	float hDiff = section->h - rect.h;

	LOG("hDiff %f",hDiff);

	rect.x = x;
	rect.y = (int)(y + (hDiff/2));
	return rect;
}


void ModuleRender::SetAlphaLineParametersPercentual(float percent) {
	alphaLineDistanceStart = ALPHA_DISTANCE_MIN + (percent*(ALPHA_DISTANCE_MAX - ALPHA_DISTANCE_MIN));
	alphaLineSizeStart = ALPHA_SIZE_MIN + (percent*(ALPHA_SIZE_MAX - ALPHA_SIZE_MIN));
}


void ModuleRender::SetBackgroundParametersPercentual(float percent) {
	backgroundSpeed = BACKGROUND_SPEED_MIN + (percent*(BACKGROUND_SPEED_MAX - BACKGROUND_SPEED_MIN));
	stageSpeed = STAGE_SPEED_MIN + (percent*(STAGE_SPEED_MAX - STAGE_SPEED_MIN));
}


float ModuleRender::DepthScale(float z) {
	float dist = nearClippingPlane + z;

	if (dist == 0)
		return 0;


	return nearClippingPlane / dist;
}