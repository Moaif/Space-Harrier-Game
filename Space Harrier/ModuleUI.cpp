#include "Application.h"
#include "ModuleUI.h"
#include "ModuleTextures.h"
#include "ModuleFont.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"
#include "ModuleScene.h"
#include "ModuleInput.h"
#include "ModuleFadeToBlack.h"
#include "ModuleMenu.h"
#include "Font.h"
#include "json.hpp"
#include <fstream>

using json = nlohmann::json;


const float ModuleUI::SEPARATION_OFFSET = 10.0f;
const float ModuleUI::TOPSCORE_X_POS = -120.0f;
const float ModuleUI::SCORE_X_POS = 40.0f;
const float ModuleUI::STAGE_X_POS = 100.0f;
const float ModuleUI::TOP_ELEMENTS_Y_POS=208;
const float ModuleUI::BOT_ELEMENTS_Y_POS=1;
const float ModuleUI::TIME_WITH_TITLE = 5.0f;
const long ModuleUI::POINTS_PER_SECOND = 100;
const float ModuleUI::POINTS_ACTUALIZATION_PER_SECOND = 10;
const float ModuleUI::CONGRATS_TIME = 5;
const float ModuleUI::END_TIME = 8;
const float ModuleUI::END_MAX_SIZE = 5;
const int ModuleUI::MAX_SCOREBOARD = 7;
const long ModuleUI::DEFAULT_SCORE = 100000;
const float ModuleUI::SCOREBOARD_TEXT_SEPARATION = 10;
const float ModuleUI::SCOREBOARD_COLUMN_SEPARATION = 70;
const int ModuleUI::MAX_LETTER_PER_NAME = 3;
const float ModuleUI::LETTER_INTERVAL = 0.25f;

ModuleUI::ModuleUI(bool active):Module(active) {
	topScore = {9,15,32,14};
	score = {1,0,48,14};
	liveImg = {20,31,10,16};
}

ModuleUI::~ModuleUI() {

}

bool ModuleUI::Start() {
	LOG("Loading UI textures");
	graphics = App->textures->Load("assets/UI.png");
	ASSERT(graphics != nullptr,AT("Failed on loading UI textures"));
	red = App->fonts->GetFont("Red", __FILE__, redFontLineReference=__LINE__);
	ASSERT(red != nullptr,AT("Failed on getting red Font"));
	blue = App->fonts->GetFont("Blue", __FILE__, blueFontLineReference=__LINE__);
	ASSERT(blue != nullptr, AT("Failed on getting blue Font"));
	green = App->fonts->GetFont("Green", __FILE__,greenFontLineReference=__LINE__);
	ASSERT(green != nullptr, AT("Failed on getting green Font"));
	yellow = App->fonts->GetFont("Yellow", __FILE__,yellowFontLineReference=__LINE__);
	ASSERT(yellow != nullptr, AT("Failed on getting yellow Font"));

	startTitleTimer = 0.0f;
	pointsTimer = 0.0f;

	return true;
}

bool ModuleUI::Restart() {
	topPoints = GetTopScore();
	points = 0;
	actualPlayerName = "___";
	endGameTimer = 0.0f;
	scoreB = false;
	playerPosInScore = -1;
	actualLetterSelected = 65;
	actualLetterInName = 0;
	letterTimer = 0.0f;

	return true;
}

bool ModuleUI::CleanUp() {
	LOG("Unloading UI textures");
	App->textures->Unload(graphics);

	App->fonts->FreeFont(&red,__FILE__,redFontLineReference);
	App->fonts->FreeFont(&blue,__FILE__,blueFontLineReference);
	App->fonts->FreeFont(&green,__FILE__,greenFontLineReference);
	App->fonts->FreeFont(&yellow,__FILE__,yellowFontLineReference);

	return true;
}

update_status ModuleUI::Update() {
	if (congrat) {
		if (endGameTimer <= CONGRATS_TIME) {
			float size = 1.5f;
			App->renderer->Print(blue, 0, (SCREEN_HEIGHT / 2) + (blue->GetYSize()*size), "CONGRATULATIONS", size);
			App->renderer->Print(blue, 0, (SCREEN_HEIGHT / 2) - (blue->GetYSize()*size), "YOU WIN", size);
		}
		else
		{
			congrat = false;
			endGameTimer = 0.0f;
		}
		endGameTimer += App->time->GetDeltaTime();
	}

	if (end) {
		if (endGameTimer <= END_TIME) {
			App->renderer->Print(yellow, 0, (SCREEN_HEIGHT / 2) + (yellow->GetYSize()*(endSize - 1)), "THE", (endSize - 1));
			App->renderer->Print(yellow, 0, (SCREEN_HEIGHT / 2) - (yellow->GetYSize()*endSize), "END", endSize);
			endSize = endGameTimer * END_MAX_SIZE / END_TIME;
		}
		else
		{
			App->scene->End();
			end = false;
			endGameTimer = 0.0f;
		}
		endGameTimer += App->time->GetDeltaTime();
	}

	if (scoreB) {
		ScoreBoard();
		return UPDATE_CONTINUE;//We dont want to print anything else
	}

	//Start Stage Title
	string stageTemp = to_string(App->scene->currentStage);
	if (startTitleTimer < TIME_WITH_TITLE) {
		float titleScale = 2.0f;
		App->renderer->Print(blue, 0, (SCREEN_HEIGHT / 2) + (blue->GetYSize()*titleScale), "STAGE " + stageTemp, titleScale);
		App->renderer->Print(blue, 0, (SCREEN_HEIGHT / 2) - (blue->GetYSize()*titleScale), App->scene->stageName, titleScale);
		startTitleTimer += App->time->GetDeltaTime();
	}
	//Stage message
	App->renderer->Print(blue, STAGE_X_POS, 1, "STAGE");
	float stageposX = STAGE_X_POS + (score.w / 2) + (0.5f * blue->GetXSize())*stageTemp.size() + SEPARATION_OFFSET / 2;
	App->renderer->Print(blue, stageposX, BOT_ELEMENTS_Y_POS, stageTemp);


	//TopScore
	App->renderer->AddToBlitBuffer(graphics, TOPSCORE_X_POS, TOP_ELEMENTS_Y_POS, FONTS_Z, &topScore, nullptr);
	float topScoreposY = TOP_ELEMENTS_Y_POS + ((topScore.h - red->GetYSize()) / 2);
	if (points > topPoints) {
		string temp = to_string(points);
		float topScoreposX = TOPSCORE_X_POS + (topScore.w / 2) + ((0.5f * red->GetXSize())*temp.size()) + SEPARATION_OFFSET;
		App->renderer->Print(red, topScoreposX, topScoreposY, temp);
	}
	else
	{
		string temp = to_string(topPoints);
		float topScoreposX = TOPSCORE_X_POS + (topScore.w / 2) + (0.5f * red->GetXSize())*temp.size() + SEPARATION_OFFSET;
		App->renderer->Print(red, topScoreposX, topScoreposY, temp);
	}

	//Score
	App->renderer->AddToBlitBuffer(graphics, SCORE_X_POS, 208, FONTS_Z, &score, nullptr);
	string scoreTemp = to_string(points);
	float scoreposY = TOP_ELEMENTS_Y_POS + ((score.h - green->GetYSize()) / 2);
	float scoreposX = SCORE_X_POS + (score.w / 2) + (0.5f * green->GetXSize())*scoreTemp.size() + SEPARATION_OFFSET;
	App->renderer->Print(green, scoreposX, scoreposY, scoreTemp);

	//Lives
	for (int i = 0; i < App->player->GetLives(); ++i) {
		App->renderer->AddToBlitBuffer(graphics, -120.0f - (i*(liveImg.w + 1)), BOT_ELEMENTS_Y_POS, FONTS_Z, &liveImg, nullptr);
	}

	//Increase points by time
	while (pointsTimer >= 1 / POINTS_ACTUALIZATION_PER_SECOND) {
		points +=(long)( POINTS_PER_SECOND / POINTS_ACTUALIZATION_PER_SECOND);
		pointsTimer -= 1 / POINTS_ACTUALIZATION_PER_SECOND;
	}
	pointsTimer += App->time->GetDeltaTime();


	return UPDATE_CONTINUE;
}

void ModuleUI::AddPoints(const int& value) {
	points += value;
}

int ModuleUI::GetPoints() const {
	return points;
}

void ModuleUI::Congratulations() {
	congrat = true;
	endGameTimer = 0.0f;
}
void ModuleUI::TheEnd() {
	end = true;
	endGameTimer = 0.0f;
	endSize = 0;
}

void ModuleUI::PauseMenu() {
	App->renderer->DirectPrint(red,0,SCREEN_HEIGHT/2,"PAUSE",2);
}

void ModuleUI::ShowLevelTitle() {
	startTitleTimer = 0.0f;
}

void ModuleUI::SetScoreBoard(bool active) {

	scoreB = active;

	if (active) {
		json input;
		ifstream ifs("assets/json/Scores.json");
		bool placed = false;
		scores.clear();
		if (ifs.fail()) {
			LOG("The file Scores.json could not be found in it's directory");
			for (int i = 0; i < MAX_SCOREBOARD; ++i) {
				if (points > DEFAULT_SCORE && !placed) {
					scores.push_back({ actualPlayerName,points });
					playerPosInScore = i;
					placed = true;
				}
				else
				{
					scores.push_back({ "ABC",DEFAULT_SCORE });
				}
			}
			return;
		}
		ifs >> input;
		int scoreOffset = 0;
		for (int i = 0; i < MAX_SCOREBOARD;++i) {
			long tempValue = input[to_string(scoreOffset)]["Score"];
			if (points > tempValue  && !placed) {
				scores.push_back({ actualPlayerName,points });
				playerPosInScore = i;
				placed = true;
			}
			else
			{
				string tempS = input[to_string(scoreOffset)]["Name"];
				scores.push_back({ tempS,tempValue  });
				++scoreOffset;
			}
		}
	}
}

void ModuleUI::ScoreBoard() {
	float lastYPos = SCREEN_HEIGHT - (SCOREBOARD_TEXT_SEPARATION+blue->GetYSize());
	App->renderer->Print(blue,0,lastYPos,"RANKING LIST");
	
	lastYPos -= (SCOREBOARD_TEXT_SEPARATION+blue->GetYSize());
	App->renderer->Print(blue,0,lastYPos,"SCORE");
	App->renderer->Print(blue,SCOREBOARD_COLUMN_SEPARATION,lastYPos,"NAME");

	for (int i = 0; i < MAX_SCOREBOARD; ++i) {
		lastYPos -= (SCOREBOARD_TEXT_SEPARATION + blue->GetYSize());
		App->renderer->Print(blue,-SCOREBOARD_COLUMN_SEPARATION,lastYPos,to_string(i+1)+".");
		App->renderer->Print(blue,0,lastYPos,to_string(scores[i].score));
		if (playerPosInScore == i) {
			App->renderer->Print(yellow, SCOREBOARD_COLUMN_SEPARATION, lastYPos, actualPlayerName);
		}
		else
		{
			App->renderer->Print(yellow, SCOREBOARD_COLUMN_SEPARATION, lastYPos, scores[i].name);
		}
	}

	//If player in scoreboard
	if (playerPosInScore != -1) {
		float actualPosX = -(((yellow->GetXSize() * 27.0f)/2.0f)-(yellow->GetXSize()/2.0f));//27 letters in alphabet
		lastYPos -= (SCOREBOARD_TEXT_SEPARATION + yellow->GetYSize());
		//Print alphabet 65 = A, 90 = Z
		for (int i = 65; i <= 90; ++i) {
			char tempC = i;
			string message;
			message.push_back(tempC);
			if (actualLetterSelected == i) {
				actualPosX += red->GetXSize();
				App->renderer->Print(red,actualPosX,lastYPos,message);
			}
			else
			{
				actualPosX += yellow->GetXSize();
				App->renderer->Print(yellow, actualPosX, lastYPos, message);
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN) {
			if (++actualLetterSelected > 90) {
				actualLetterSelected = 65;
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN) {
			if (--actualLetterSelected < 65) {
				actualLetterSelected = 90;
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_BACKSPACE) == KEY_DOWN) {
			if (actualLetterInName > 0) {
				actualPlayerName[actualLetterInName] = '_';
				actualPlayerName[actualLetterInName - 1] = '_';
				--actualLetterInName;
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
			if (actualLetterInName == MAX_LETTER_PER_NAME) {
				scores[playerPosInScore].name = actualPlayerName;
				Write();
				App->fade->FadeToBlack(App->menu,App->scene);
			}
			else
			{
				actualPlayerName[actualLetterInName] = ((char)actualLetterSelected);
				++actualLetterInName;
			}
		}
		//Show actualLetter on player name at intervals
		float actualTime = App->time->GetTimeSinceStart();
		if (( actualTime- letterTimer) >= LETTER_INTERVAL) {
			actualPlayerName[actualLetterInName] = (char)actualLetterSelected;
			if ((actualTime - letterTimer) > LETTER_INTERVAL * 2) {
				actualPlayerName[actualLetterInName] = '_';
				letterTimer = actualTime;
			}
		}
	}
	else {
		if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {
			App->fade->FadeToBlack(App->menu, App->scene);
		}
	}
}

void ModuleUI::Write() {
	json output;

	for (int i = 0; i < MAX_SCOREBOARD; ++i) {
		output[to_string(i)]["Name"] = scores[i].name;
		output[to_string(i)]["Score"] = scores[i].score;
	}

	ofstream ofs("assets/json/Scores.json");
	ofs << setw(4) << output << endl;
}

long ModuleUI::GetTopScore() const {
	json input;
	ifstream ifs("assets/json/Scores.json");
	if (ifs.fail()) {
		LOG("The file Scores.json could not be found in it's directory");
		return DEFAULT_SCORE;
	}
	ifs >> input;
	long tempL = input["0"]["Score"];
	return tempL;
}