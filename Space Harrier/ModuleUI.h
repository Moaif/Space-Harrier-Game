#ifndef _MODULEUI_H_
#define _MODULEUI_H_

#include "Module.h"
#include "Globals.h"

struct SDL_Texture;
class Font;

class ModuleUI :public Module {
public:
	ModuleUI(bool active=true);
	~ModuleUI();

	bool Start();
	update_status Update();
	bool CleanUp();

	void AddPoints(int value);
	int GetPoints();

private:
	long points;
	long topPoints;
	float startTitleTimer;
	float pointsTimer;
	SDL_Texture* graphics;
	SDL_Rect topScore;
	SDL_Rect score;
	SDL_Rect liveImg;
	const Font* red;
	const Font* blue;
	const Font* green;
	const Font * yellow;
	int redFontLineReference;
	int blueFontLineReference;
	int greenFontLineReference;
	int yellowFontLineReference;
	static const float SEPARATION_OFFSET;
	static const float TOPSCORE_X_POS;
	static const float SCORE_X_POS;
	static const float STAGE_X_POS;
	static const float TIME_WITH_TITLE;
	static const int POINTS_PER_SECOND;
	static const float POINTS_ACTUALIZATION_PER_SECOND;
};

#endif // !_MODULEUI_H_

