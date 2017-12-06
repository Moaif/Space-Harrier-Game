#include "Application.h"
#include "ModuleUI.h"
#include "ModuleTextures.h"
#include "ModuleFont.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"
#include "ModuleScene.h"
#include <string>
#include "Font.h"

const float ModuleUI::SEPARATION_OFFSET = 10.0f;
const float ModuleUI::TOPSCORE_X_POS = -120.0f;
const float ModuleUI::SCORE_X_POS = 40.0f;
const float ModuleUI::STAGE_X_POS = 100.0f;
const float ModuleUI::TOP_ELEMENTS_Y_POS=208;
const float ModuleUI::BOT_ELEMENTS_Y_POS=1;
const float ModuleUI::TIME_WITH_TITLE = 5.0f;
const int ModuleUI::POINTS_PER_SECOND = 100;
const float ModuleUI::POINTS_ACTUALIZATION_PER_SECOND = 10;

ModuleUI::ModuleUI(bool active):Module(active) {
	topScore = {1,0,38,15};
	score = {40,0,60,15};
	liveImg = {105,2,6,10};
	points = 0;
	topPoints = 100000;
	pointsTimer = 0;
}

ModuleUI::~ModuleUI() {

}

bool ModuleUI::Start() {
	LOG("Loading UI textures");
	graphics = App->textures->Load("assets/UI.png");
	red = App->fonts->GetFont("Red", __FILE__, redFontLineReference=__LINE__);
	blue = App->fonts->GetFont("Blue", __FILE__, blueFontLineReference=__LINE__);
	green = App->fonts->GetFont("Green", __FILE__,greenFontLineReference=__LINE__);
	yellow = App->fonts->GetFont("Yellow", __FILE__,yellowFontLineReference=__LINE__);

	startTitleTimer = 0.0f;

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
	//Start Stage Title
	string stageTemp = to_string(App->scene->currentStage);
	if (startTitleTimer < TIME_WITH_TITLE) {
		App->renderer->Print(blue,0,(SCREEN_HEIGHT/2)+blue->GetYSize(),"STAGE "+stageTemp,2);
		App->renderer->Print(blue,0,(SCREEN_HEIGHT/2)-blue->GetYSize(),App->scene->stageName,2);
		startTitleTimer += App->time->GetDeltaTime();
	}
	//Stage message
	App->renderer->Print(blue, STAGE_X_POS, 1, "STAGE");
	float stageposX = STAGE_X_POS + (score.w / 2) + (0.5f * blue->GetXSize())*stageTemp.size() + SEPARATION_OFFSET/2;
	App->renderer->Print(blue,stageposX,BOT_ELEMENTS_Y_POS,stageTemp);


	//TopScore
	App->renderer->AddToBlitBuffer(graphics,TOPSCORE_X_POS,TOP_ELEMENTS_Y_POS,FONTS_Z,&topScore,nullptr);
	float topScoreposY=TOP_ELEMENTS_Y_POS+((topScore.h-red->GetYSize())/2);
	if (points > topPoints) {
		string temp = to_string(points);
		float topScoreposX = TOPSCORE_X_POS + (topScore.w / 2) + ((0.5f * red->GetXSize())*temp.size())+SEPARATION_OFFSET;
		App->renderer->Print(red,topScoreposX,topScoreposY,temp);
	}
	else
	{
		string temp = to_string(topPoints);
		float topScoreposX = TOPSCORE_X_POS + (topScore.w / 2) + (0.5f * red->GetXSize())*temp.size()+SEPARATION_OFFSET;
		App->renderer->Print(red,topScoreposX,topScoreposY,temp);
	}

	//Score
	App->renderer->AddToBlitBuffer(graphics, SCORE_X_POS, 208, FONTS_Z, &score, nullptr);
	string scoreTemp = to_string(points);
	float scoreposY = TOP_ELEMENTS_Y_POS + ((score.h - green->GetYSize()) / 2);
	float scoreposX = SCORE_X_POS + (score.w / 2) + (0.5f * green->GetXSize())*scoreTemp.size() + SEPARATION_OFFSET;
	App->renderer->Print(green,scoreposX,scoreposY,scoreTemp);

	//Lives
	for (int i = 0; i < App->player->GetLives(); ++i) {
		App->renderer->AddToBlitBuffer(graphics, -120-(i*(liveImg.w+1)), BOT_ELEMENTS_Y_POS, FONTS_Z, &liveImg, nullptr);
	}

	//Increase points by time
	while (pointsTimer >= 1/POINTS_ACTUALIZATION_PER_SECOND) {
		points += POINTS_PER_SECOND/POINTS_ACTUALIZATION_PER_SECOND;
		pointsTimer -= 1/POINTS_ACTUALIZATION_PER_SECOND;
	}
	pointsTimer += App->time->GetDeltaTime();

	return UPDATE_CONTINUE;
}

void ModuleUI::AddPoints(int value) {
	points += value;
}

int ModuleUI::GetPoints() {
	return points;
}