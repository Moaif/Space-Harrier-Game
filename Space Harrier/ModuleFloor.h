#ifndef _MODULEFLOOR_H_
#define _MODULEFLOOR_H_

#include "Module.h"
#include "Point.h"
#include "Globals.h"

struct SDL_Texture;
struct SDL_Rect;

struct obstacleInfo
{
	float y;
	float h;
};

class ModuleFloor:public Module{
public:
	ModuleFloor();
	~ModuleFloor();

	bool DrawFloor(SDL_Texture* texture);
	bool DrawBackground(SDL_Texture* texture);
	bool DrawStage(SDL_Texture* texture);
	void SetBackgroundParametersPercentual(const float& percent);
	void SetHorizonYPerccentual(const float& percen);
	const obstacleInfo* GetQuad(const int& index)const ;
	float GetFloorPositionFromZ(const float& z)const;

private:
	void DrawAlphaLines();

public:
	fPoint horizon;
	float actualCameraY;
	static const int nHorizonQuads = 11;
	bool nextFrameItChange = false;//Bool to know if the alpha lines quads indexes are going to change in next frame


private:
	//Horizon
	static const float HORIZON_Y_MAX;
	static const float HORIZON_Y_MIN;

	//Floor
	float increasePixelIteration = 0.0f;
	float alphaDivisor;
	float firstSegmentPositionPercentage;
	static const float SEGMENT_REDUCTION;
	static const float ALPHA_LINES_SPEED;
	SDL_Rect quads[nHorizonQuads];
	obstacleInfo quad2[nHorizonQuads];
	int lastQuadIndex;


	//Background
	float backgroundOffset = 0.0f;
	float stageOffset = 0.0f;
	float backgroundSpeed = 0.0f;
	float stageSpeed = 0.0f;
	static const float STAGE_SPEED_MAX;
	static const float STAGE_SPEED_MIN;
	static const float BACKGROUND_SPEED_MAX;
	static const float BACKGROUND_SPEED_MIN;

	//Camera parameters for z-calculation
	float cameraYMax;
	float cameraYMin;
};

#endif // !_MODULEFLOOR_H_
