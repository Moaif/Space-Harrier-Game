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
#include "ModuleSceneSpace.h"

// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA

ModulePlayer::ModulePlayer(bool active) : Module(active)
{
	// runing animation
	run.frames.push_back({ 4, 4, 20, 47 });
	run.frames.push_back({ 25, 4, 20, 47 });
	run.frames.push_back({ 49, 2, 25, 49 });
	run.frames.push_back({ 75, 3, 21, 47 });
	run.speed = 0.05f;

	center.frames.push_back({ 108,2,26,49 });

	left1.frames.push_back({ 142,2,22,50 });

	left2.frames.push_back({ 170,3,20,48});

	right1.frames.push_back({ 221,3,22,49});
	right2.frames.push_back({ 197,3,20,48 });

	current_animation = &run;
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
	collider = App->collision->AddCollider({ (int)position.x,(int)position.y,25,50 },position.z, PLAYER, this);

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
	int movement_speed = 3;

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		if (position.x > ((-SCREEN_WIDTH/2)+current_animation->GetCurrentFrame().w/2)) {
			position.x -= movement_speed;
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
			position.x += movement_speed;
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
			position.y -= movement_speed;
			if (position.y <= 0) {
				current_animation = &run;
			}
			VerifyHorizonY();
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		if (position.y < (SCREEN_HEIGHT-current_animation->GetCurrentFrame().h)) {
			position.y += movement_speed;
			if (current_animation == &run) {
				VerifyFlyAnimation();
			}
			VerifyHorizonY();
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		float x =(float) position.x;
		float y =(float) position.y + (current_animation->GetCurrentFrame().h/2);
		App->particles->AddParticle(App->particles->laser, x, y);
	}

	// Draw everything --------------------------------------
	if (destroyed == false) {
		collider->rect.x =(int) position.x;
		collider->rect.y =(int) position.y;
		SDL_Rect screenPos = App->renderer->ToScreenPoint(position.x,position.y,position.z,&(current_animation->GetCurrentFrame()));
		App->renderer->AddToBlitBuffer(graphics, screenPos.x, screenPos.y, PLAYER_Z,&(current_animation->GetCurrentFrame()),&screenPos);
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

	if ((position.x + (SCREEN_WIDTH / 2)) <= (SCREEN_WIDTH / 5)) {
		current_animation = &left2;
		CalculateSpeed();
		return;
	}
	if ((position.x + (SCREEN_WIDTH / 2))<= (SCREEN_WIDTH / 5*2)) {
		current_animation = &left1;
		CalculateSpeed();
		return;
	}
	if ((position.x + (SCREEN_WIDTH / 2)) <= (SCREEN_WIDTH / 5*3)) {
		current_animation = &center;
		speed=0;
		return;
	}
	if ((position.x + (SCREEN_WIDTH / 2)) <= (SCREEN_WIDTH / 5*4)) {
		current_animation = &right1;
		CalculateSpeed();
		return;
	}
	if ((position.x + (SCREEN_WIDTH/2)) <= (SCREEN_WIDTH )) {
		current_animation = &right2;
		CalculateSpeed();
		return;
	}
}

void ModulePlayer::VerifyFloorSpeed() {

	if ((position.x + (SCREEN_WIDTH / 2)) <= (SCREEN_WIDTH / 5 * 2)) {
		CalculateSpeed();
		return;
	}
	if ((position.x + (SCREEN_WIDTH / 2)) <= (SCREEN_WIDTH / 5 * 3)) {
		speed=0;
		return;
	}
	if ((position.x + (SCREEN_WIDTH / 2)) <= (SCREEN_WIDTH)) {
		CalculateSpeed();
		return;
	}
}

void ModulePlayer::VerifyHorizonX() {
	
	//Calculate percentual position from character
	float temp = (position.x + (SCREEN_WIDTH / 2))/SCREEN_WIDTH;
	App->renderer->SetBackgroundParametersPercentual(temp);

}

void ModulePlayer::VerifyHorizonY() {

	//Calculate percentual position from character
	float temp = position.y / (SCREEN_HEIGHT - current_animation->GetCurrentFrame().h);
	App->renderer->horizon.y = (int)(HORIZON_Y_MIN + (temp*(HORIZON_Y_MAX - HORIZON_Y_MIN)));
	App->renderer->SetAlphaLineParametersPercentual(temp);
}

void ModulePlayer::CalculateSpeed() {
	float temp = (position.x + (SCREEN_WIDTH / 2)) / SCREEN_WIDTH;
	speed = MIN_X_SPEED + (temp*(MAX_X_SPEED - MIN_X_SPEED));
}