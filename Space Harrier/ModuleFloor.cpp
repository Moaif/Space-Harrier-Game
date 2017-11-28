#include "Application.h"
#include "ModuleFloor.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"
#include "ModuleTime.h"
#include "SDL/include/SDL.h"

const float ModuleFloor::SEGMENT_REDUCTION = 0.60f;
const float ModuleFloor::ALPHA_LINES_SPEED = 3.0f;
const float ModuleFloor::HORIZON_Y_MAX = 100;
const float ModuleFloor::HORIZON_Y_MIN = 20;
const float ModuleFloor::STAGE_SPEED_MAX = 60;
const float ModuleFloor::STAGE_SPEED_MIN = -60;
const float ModuleFloor::BACKGROUND_SPEED_MAX = 30;
const float ModuleFloor::BACKGROUND_SPEED_MIN = -30;

ModuleFloor::ModuleFloor() {
	horizon = { 0,HORIZON_Y_MIN };
	alphaDivisor = 1;
	for (int i = 1; i < nHorizonQuads; ++i) {
		alphaDivisor += pow(SEGMENT_REDUCTION, i);
	}
	firstSegmentPositionPercentage = 0.0f;
	for (int i = 0; i < nHorizonQuads; i++) {
		quads[i].x = 0;
		quads[i].w = SCREEN_SIZE*SCREEN_WIDTH;
	}
	lastQuadIndex = nHorizonQuads-1;

	cameraYMax = (float)HORIZON_Y_MAX*CLIPDISTANCE / MAX_Z;
	cameraYMin = (float)HORIZON_Y_MIN*CLIPDISTANCE / MAX_Z;
	actualCameraY = cameraYMin;//Because it starts at bottom;
}

ModuleFloor::~ModuleFloor() {

}

bool ModuleFloor::DrawFloor(SDL_Texture* texture)
{
	bool ret = true;
	SDL_Rect rect;
	int textW, textH;

	SDL_QueryTexture(texture, NULL, NULL, &textW, &textH);

	rect.w = SCREEN_WIDTH;
	rect.h = (int) horizon.y;

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
	increasePixelIteration += App->player->speedStage*App->time->GetDeltaTime();

	float pixelsPerRow = (float)textH / rect.h;
	float pixelsPerRowOffset = 0.0f;

	SDL_Rect textureLine = { (int)maxAditionalPixelsX, 0, (int)(textW - maxAditionalPixelsX * 2), 1 };
	rect.h = 1;

	int originalRectX = textureLine.x;
	float deviation = 0.0f;

	for (int i = 0; i <= (horizon.y*SCREEN_SIZE); ++i)
	{
		deviation = (((float)i / (horizon.y*(float)SCREEN_SIZE))*increasePixelIteration);
		textureLine.x = originalRectX + (int)deviation;
		if (SDL_RenderCopy(App->renderer->renderer, texture, &textureLine, &rect) != 0) {
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

void ModuleFloor::DrawAlphaLines()
{
	float baseSegmentHeight = (float)(horizon.y*SCREEN_SIZE) / alphaDivisor;

	float startingRenderingPosition = SCREEN_HEIGHT*SCREEN_SIZE - baseSegmentHeight*(1.0f - firstSegmentPositionPercentage);
	float firstSegmentHeight = baseSegmentHeight * (1.0f - firstSegmentPositionPercentage) + baseSegmentHeight * (1.0f / SEGMENT_REDUCTION) * (firstSegmentPositionPercentage);

	float currentSegmentHeight = firstSegmentHeight;
	float currentRenderingPosition = startingRenderingPosition;

	int currentQuad = (lastQuadIndex+1) %nHorizonQuads;
	bool hasLoopedArray = false;
	do {
		float currentSegmentPrintedHeight = currentSegmentHeight * (1.0f - SEGMENT_REDUCTION);
		quads[currentQuad].y = (int)currentRenderingPosition;
		quads[currentQuad].h = (int)currentSegmentPrintedHeight;
		quad2[currentQuad].y = currentRenderingPosition;
		quad2[currentQuad].h = currentSegmentPrintedHeight;
		currentSegmentHeight = currentSegmentHeight * SEGMENT_REDUCTION;
		currentRenderingPosition -= currentSegmentHeight;

		currentQuad = (currentQuad + 1) % nHorizonQuads;
		hasLoopedArray = currentQuad == (lastQuadIndex + 1) % nHorizonQuads;
	} while (!hasLoopedArray);

	App->renderer->DrawQuads(quads, nHorizonQuads, 0, 0, 0, 50);

	float nextfirstSegmentPositionPercentage = fmod(firstSegmentPositionPercentage + (ALPHA_LINES_SPEED*App->time->GetDeltaTime()), 1.0f);
	if (nextfirstSegmentPositionPercentage < firstSegmentPositionPercentage) {
		lastQuadIndex = (lastQuadIndex + 1) % nHorizonQuads;
	}
	firstSegmentPositionPercentage = nextfirstSegmentPositionPercentage;
}

bool ModuleFloor::DrawBackground(SDL_Texture* texture) {
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
		rect.w = SCREEN_WIDTH - rect2.w + 1;//With round, somethimes it leaves 1 pixel without drawing, thats why we add +1
		if (!App->renderer->Blit(texture, (-(SCREEN_WIDTH / 2)) + (rect.w / 2.0f), horizon.y-1, &rect, nullptr)) {
			return false;
		}
		ret = App->renderer->Blit(texture, (SCREEN_WIDTH / 2) - (rect2.w / 2.0f), horizon.y-1, &rect2, nullptr);
	}
	else
	{
		ret = App->renderer->Blit(texture, 0, horizon.y-1, &rect, nullptr);
	}

	backgroundOffset += backgroundSpeed*App->time->GetDeltaTime();
	if (backgroundOffset >= w) {
		backgroundOffset = 0;
	}
	if (backgroundOffset < 0) {
		backgroundOffset = (float)w;
	}

	return ret;
}

bool ModuleFloor::DrawStage(SDL_Texture* texture) {
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
		if (!App->renderer->Blit(texture, (-(SCREEN_WIDTH / 2)) + (rect.w / 2.0f), horizon.y-1, &rect, nullptr)) {
			return false;
		}
		ret = App->renderer->Blit(texture, (SCREEN_WIDTH / 2) - (rect2.w / 2.0f), horizon.y-1, &rect2, nullptr);
	}
	else
	{
		ret = App->renderer->Blit(texture, 0, horizon.y-1, &rect, nullptr);
	}

	stageOffset += stageSpeed*App->time->GetDeltaTime();
	if (stageOffset >= w) {
		stageOffset = 0;
	}
	if (stageOffset < 0) {
		stageOffset = (float)w;
	}

	return ret;
}

void ModuleFloor::SetBackgroundParametersPercentual(float percent) {
	backgroundSpeed = BACKGROUND_SPEED_MIN + (percent*(BACKGROUND_SPEED_MAX - BACKGROUND_SPEED_MIN));
	stageSpeed = STAGE_SPEED_MIN + (percent*(STAGE_SPEED_MAX - STAGE_SPEED_MIN));
}

void ModuleFloor::SetHorizonYPerccentual(float percen) {
	horizon.y=(HORIZON_Y_MIN + (percen*(HORIZON_Y_MAX - HORIZON_Y_MIN)));
	actualCameraY = (cameraYMin +(percen*(cameraYMax - cameraYMin)));
}

const obstacleInfo* ModuleFloor::GetQuad(int index)const  {
	int innerIndex = (lastQuadIndex + index)%nHorizonQuads;
	return &quad2[innerIndex];
}

float ModuleFloor::GetFloorPositionFromZ(const float& z) const{
	return z*actualCameraY / CLIPDISTANCE;
}
