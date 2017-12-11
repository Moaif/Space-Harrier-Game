#ifndef _MODULEUI_H_
#define _MODULEUI_H_

#include "Module.h"
#include "Globals.h"
#include <vector>

struct SDL_Texture;
class Font;

struct ScoreInfo {
	std::string name;
	long score;
};

class ModuleUI :public Module {
public:
	ModuleUI(bool active=true);
	~ModuleUI();

	bool Start();
	update_status Update();
	bool CleanUp();

	void AddPoints(int value);
	int GetPoints();

	void Congratulations();
	void TheEnd();

	void PauseMenu();

	void SetCountingPoints(bool active);
	void SetScoreBoard(bool active);
private:
	void ScoreBoard();
	void Write();
	long GetTopScore();
private:
	long points;
	bool countingPoints = true;
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
	static const float TOP_ELEMENTS_Y_POS;
	static const float BOT_ELEMENTS_Y_POS;
	static const float TIME_WITH_TITLE;
	static const int POINTS_PER_SECOND;
	static const float POINTS_ACTUALIZATION_PER_SECOND;
	static const float CONGRATS_TIME;
	static const float END_TIME;
	static const float END_MAX_SIZE;
	static const int MAX_SCOREBOARD;
	static const long DEFAULT_SCORE;
	static const float SCOREBOARD_TEXT_SEPARATION;
	static const float SCOREBOARD_COLUMN_SEPARATION;
	static const int MAX_LETTER_PER_NAME;
	//End game
	bool congrat = false;
	bool end = false;
	float endSize;
	float endGameTimer;
	//ScoreBoard
	bool scoreB = false;
	std::vector<ScoreInfo> scores;
	int playerPosInScore;
	std::string actualPlayerName;
	int actualLetterInName;
	int actualLetterSelected;
};

#endif // !_MODULEUI_H_

