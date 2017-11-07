#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "SDL/include/SDL.h"

ModuleRender::ModuleRender()
{
	camera.x = camera.y = 0;
	camera.w = SCREEN_WIDTH * SCREEN_SIZE;
	camera.h = SCREEN_HEIGHT* SCREEN_SIZE;
	horizon = { 0,HORIZON_Y_MIN };
	alphaLineDistanceStart = ALPHA_DISTANCE_MIN;
	alphaLineSizeStart = ALPHA_SIZE_MIN;
	nearClippingPlane = 3;
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
	// debug camera
	int speed = 1;

	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->renderer->camera.y += speed;

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->renderer->camera.y -= speed;

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->renderer->camera.x += speed;

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->renderer->camera.x -= speed;

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

bool ModuleRender::DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera)
{
	bool ret = true;

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	SDL_Rect rec(rect);
	if (use_camera)
	{
		rec.x = (int)(camera.x + rect.x * SCREEN_SIZE);
		rec.y = (int)(camera.y + rect.y * SCREEN_SIZE);
		rec.w *= SCREEN_SIZE;
		rec.h *= SCREEN_SIZE;
	}

	if (SDL_RenderFillRect(renderer, &rec) != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

void ModuleRender::DrawAlphaLines()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 50);
	alphaLineDistance = alphaLineDistanceStart;
	alphaLineSize = alphaLineSizeStart;
	float coef = alphaLineIteration / alphaLineDistance;
	float offsetDif = 0;

	while (alphaLineDistance <= (horizon.y*SCREEN_SIZE))
	{
		const SDL_Rect test = { 0, SCREEN_HEIGHT*SCREEN_SIZE - (alphaLineDistance-(coef*alphaLineSize)), SCREEN_WIDTH*SCREEN_SIZE, alphaLineSize+(offsetDif*(coef/2)) };
		SDL_RenderFillRect(renderer, &test);

		offsetDif = alphaLineSize / 4.0f;
		alphaLineSize -= offsetDif;
		alphaLineDistance += (alphaLineSize * 2.0f);
	}

	alphaLineIteration = (alphaLineIteration + 2) % (int)(alphaLineDistanceStart*2);
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

void ModuleRender::SetAlphaLineParametersPercentual(float percent) {
	alphaLineDistanceStart = ALPHA_DISTANCE_MIN + (percent*(ALPHA_DISTANCE_MAX - ALPHA_DISTANCE_MIN));
	alphaLineSizeStart = ALPHA_SIZE_MIN + (percent*(ALPHA_SIZE_MAX - ALPHA_SIZE_MIN));
}


float ModuleRender::DepthScale(float z) {
	float dist = nearClippingPlane + z;

	if (dist == 0)
		return 0;


	return nearClippingPlane / dist;
}