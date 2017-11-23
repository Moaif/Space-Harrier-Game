#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleParticles.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"
#include "ModuleFadeToBlack.h"
#include "ModulePlayer.h"
#include "ModuleSceneIntro.h"
#include "ModuleScene.h"
#include "ModuleFloor.h"
#include "ModuleShadow.h"

const float ModulePlayer::MIN_X_SPEED = -15;
const float ModulePlayer::MAX_X_SPEED = 15;
const float ModulePlayer::MOVEMENT_SPEED = 300;
const float ModulePlayer::SCREEN_SEGMENT = 0.4;//5 screen zones

ModulePlayer::ModulePlayer(bool active) : Module(active)
{
	// runing animation
	run.frames.push_back({ 4, 4, 20, 47 });
	run.frames.push_back({ 25, 4, 20, 47 });
	run.frames.push_back({ 49, 2, 25, 49 });
	run.frames.push_back({ 75, 3, 21, 47 });
	run.speed = 5.0f;

	center.frames.push_back({ 108,2,26,49 });

	left1.frames.push_back({ 142,2,22,50 });

	left2.frames.push_back({ 170,3,20,48});

	right1.frames.push_back({ 221,3,22,49});
	right2.frames.push_back({ 197,3,20,48 });

	current_animation = &run;

	horizonYSpeed =0.5f;
	actualHorizonPercentage=0.0f;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	graphics = App->textures->Load("assets/character.png");

	destroyed = false;
	position.x = 0;
	position.y = 0;
	position.z = 0;
	//Collider
	collider = App->collision->AddCollider({ (int)position.x,(int)position.y,25,50 },position.z,1, PLAYER, this);

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	App->textures->Unload(graphics);

	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	VerifyHorizonY();

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		if (position.x > ((-SCREEN_WIDTH/2)+current_animation->GetCurrentFrame().w/2)) {
			position.x -= MOVEMENT_SPEED*App->time->GetDeltaTime();
			if (current_animation != &run) {
				VerifyFlyAnimation();
			}
			else
			{
				VerifyFloorSpeed();
			}
			VerifyHorizonX();
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		if (position.x < ((SCREEN_WIDTH / 2) - current_animation->GetCurrentFrame().w / 2)) {
			position.x += MOVEMENT_SPEED*App->time->GetDeltaTime();
			if (current_animation != &run) {
				VerifyFlyAnimation();
			}
			else
			{
				VerifyFloorSpeed();
			}
			VerifyHorizonX();
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		if (position.y > 0) {
			position.y -= MOVEMENT_SPEED*App->time->GetDeltaTime();
			if (position.y <= 0) {
				current_animation = &run;
			}
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		if (position.y < (SCREEN_HEIGHT-current_animation->GetCurrentFrame().h)) {
			position.y += MOVEMENT_SPEED*App->time->GetDeltaTime();
			if (current_animation == &run) {
				VerifyFlyAnimation();
			}
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		float y = position.y + (current_animation->GetCurrentFrame().h/2);
		App->particles->AddParticle(App->particles->laser, position.x, y,position.z);
	}

	// Draw everything --------------------------------------
	if (destroyed == false) {
		collider->rect.x =(int) position.x;
		collider->rect.y =(int) position.y;
		App->renderer->AddToBlitBuffer(graphics, position.x, position.y, (float)PLAYER_Z,&(current_animation->GetCurrentFrame()),nullptr);
		App->shadows->DrawShadow(position.x,0,1);
	}

	return UPDATE_CONTINUE;
}

void ModulePlayer::OnCollision(Collider * col,Collider* other) {
	if (collider == col) {
		//collider->to_delete = true;
		//destroyed = true;
	}
}

void ModulePlayer::SetLives(int amount) {
	lives = amount;
}

int ModulePlayer::GetLives() {
	return lives;
}

void ModulePlayer::VerifyFlyAnimation() {
	float pos = GetRelativeWorldPosition().x;

	if (pos <= (SCREEN_SEGMENT)-1) {
		current_animation = &left2;
		CalculateSpeed();
		return;
	}
	if (pos <= (SCREEN_SEGMENT * 2)-1) {
		current_animation = &left1;
		CalculateSpeed();
		return;
	}
	if (pos <= (SCREEN_SEGMENT * 3)-1) {
		current_animation = &center;
		speedStage=0;
		return;
	}
	if (pos <= (SCREEN_SEGMENT * 4)-1) {
		current_animation = &right1;
		CalculateSpeed();
		return;
	}
	if (pos <= (SCREEN_SEGMENT * 5)-1) {
		current_animation = &right2;
		CalculateSpeed();
		return;
	}
}

void ModulePlayer::VerifyFloorSpeed() {
	float pos = GetRelativeWorldPosition().x;
	if (pos <= (SCREEN_SEGMENT*2)-1) {
		CalculateSpeed();
		return;
	}
	if (pos <= (SCREEN_SEGMENT*3)-1) {
		speedStage=0;
		return;
	}
	if (pos <= (SCREEN_SEGMENT * 5)-1) {
		CalculateSpeed();
		return;
	}
}

void ModulePlayer::VerifyHorizonX() {
	
	//Calculate percentual position from character
	float temp = (position.x + SCREEN_WIDTH / 2) / SCREEN_WIDTH;
	App->floor->SetBackgroundParametersPercentual(temp);

}

void ModulePlayer::VerifyHorizonY() {

	//Calculate percentual position from character
	float temp = position.y / (SCREEN_HEIGHT - current_animation->GetCurrentFrame().h);
	float offset = horizonYSpeed*App->time->GetDeltaTime();
	if (actualHorizonPercentage < temp-offset) {
		actualHorizonPercentage += offset;
	}
	else if (actualHorizonPercentage > temp) {
		actualHorizonPercentage -= offset;
	}
	App->floor->SetHorizonYPerccentual(actualHorizonPercentage);
}

void ModulePlayer::CalculateSpeed() {
	float temp = (position.x +SCREEN_WIDTH/2) / SCREEN_WIDTH;
	speedStage = MIN_X_SPEED + (temp*(MAX_X_SPEED - MIN_X_SPEED));
}

fPoint ModulePlayer::GetRelativeWorldPosition() {
	fPoint p;
	p.x= (position.x*2) / (SCREEN_WIDTH);
	p.y = (((position.y) *2) / (SCREEN_HEIGHT- current_animation->GetCurrentFrame().h))-1;
	p.z = 0.0f;
	return p;
}