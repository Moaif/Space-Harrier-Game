#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleParticles.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"
#include "ModuleFadeToBlack.h"
#include "ModulePlayer.h"
#include "ModuleMenu.h"
#include "ModuleScene.h"
#include "ModuleFloor.h"
#include "ModuleShadow.h"
#include "ModuleAudio.h"
#include "ModuleUI.h"

const int ModulePlayer::INITIAL_LIVES = 3;
const float ModulePlayer::MIN_X_SPEED = -150.0f;
const float ModulePlayer::MAX_X_SPEED = 150.0f;
const float ModulePlayer::HORIZON_Y_SPEED = 0.01f;
const float ModulePlayer::MOVEMENT_SPEED = 150.0f;
const float ModulePlayer::FALL_SPEED = 2.0f;
const float ModulePlayer::FALL_BOUNCE = 15.0f;
const float ModulePlayer::RECOVER_TIME = 2.0f;
const float ModulePlayer::SCREEN_SEGMENT = 0.4f;//5 screen zones
const float ModulePlayer::COLLIDER_H = 30.0f;
const float ModulePlayer::COLLIDER_W = 10.0f;
const float ModulePlayer::WELCOME_RUN_TIME = 1.0f;
const float ModulePlayer::WELCOME_TRANSITION_TIME = 0.5f;
const float ModulePlayer::SHOOT_INTERVAL = 0.25f;

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

	death.frames.push_back({5,60,24,41});
	death.frames.push_back({ 33,60,28,42 });
	death.frames.push_back({ 72,69,28,24 });
	death.frames.push_back({ 112,73,29,22 });
	death.frames.push_back({ 157,64,28,33 });
	death.frames.push_back({ 108,2,26,49 });
	death.timeBased = false;
	death.loop = false;
	death.speed = 0.35f;

	fall.frames.push_back({5,117,24,40});
	fall.frames.push_back({ 45,119,22,35 });
	fall.frames.push_back({ 82,122,26,30 });
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	graphics = App->textures->Load("assets/character.png");

	if (ouch == 0) {
		ouch=App->audio->LoadFx("assets/music/SFX/Ouch.wav");
	}
	if (aaargh == 0) {
		aaargh=App->audio->LoadFx("assets/music/SFX/Aaargh.wav");
	}
	if (getReady == 0) {
		getReady=App->audio->LoadFx("assets/music/SFX/GetReady.wav");
	}

	recoverTimer = RECOVER_TIME;
	position.x = 0;
	position.y = 0;
	position.z = 0;
	//Collider smaller than sprites, in order to make it easier
	int y = (int)(position.y + (current_animation->GetCurrentFrame().h/2)-(COLLIDER_H / 2));
	collider = App->collision->AddCollider({ (int)position.x,y,(int)COLLIDER_W,(int)COLLIDER_H },position.z,1, PLAYER, this);

	return true;
}

bool ModulePlayer::Restart() {

	lives = INITIAL_LIVES;
	current_animation = &run;
	actualHorizonPercentage = 0.0f;
	win = false;
	centered = false;
	destroyed = false;
	welcome = false;
	welcomeTimer = 0.0f;

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
	//Welcome to the fantasy zone anim+
	if (!welcome) {
		AnimWelcome();
		return UPDATE_CONTINUE;
	}
	//If ended the game
	if (win) {
		if (position.z < MAX_Z) {
			AnimWin();
		}
		return UPDATE_CONTINUE;
	}
	//Normal game
	VerifyHorizonY();
	if (hit) 
	{
		Death();
	}
	else
	{
		//Recover from death
		if (recoverTimer >= RECOVER_TIME) {
			collider->active = true;
		}
		recoverTimer += App->time->GetDeltaTime();

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			if (position.x > ((-SCREEN_WIDTH / 2) + current_animation->GetCurrentFrame().w / 2)) {
				position.x -= MOVEMENT_SPEED*App->time->GetDeltaTime();
				if (current_animation != &run && current_animation != &fall) {
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
				if (current_animation != &run && current_animation != &fall) {
					VerifyFlyAnimation();
				}
				else
				{
					VerifyFloorSpeed();
				}
				VerifyHorizonX();
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && current_animation != &fall)
		{
			if (position.y > 0) {
				position.y -= MOVEMENT_SPEED*App->time->GetDeltaTime();
				if (position.y <= 0 ) {
					current_animation = &run;
				}
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && current_animation != &fall)
		{
			if (position.y < (SCREEN_HEIGHT - current_animation->GetCurrentFrame().h)) {
				position.y += MOVEMENT_SPEED*App->time->GetDeltaTime();
				if (current_animation == &run) {
					VerifyFlyAnimation();
				}
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && current_animation != &fall)
		{
			if (shootTimer <= 0) {
				float y = position.y + (current_animation->GetCurrentFrame().h / 2);
				App->particles->AddParticle(*App->particles->laser, position.x, y, position.z);
				shootTimer = SHOOT_INTERVAL;
			}
		}
		shootTimer -= App->time->GetDeltaTime();

		if (current_animation == &fall) {
			if (current_animation->Finished()) {
				if (position.y > 0) {
					VerifyFlyAnimation();
				}
				else
				{
					current_animation = &run;
				}
			}
		}
	}


	// Draw everything --------------------------------------

	collider->rect.x =(int) position.x;
	collider->rect.y =(int)(position.y + (current_animation->GetCurrentFrame().h / 2) - (COLLIDER_H / 2));
	App->renderer->AddToBlitBuffer(graphics, position.x, position.y, (float)PLAYER_Z,&(current_animation->GetCurrentFrame()),nullptr);
	App->shadows->DrawShadow(position.x,0,1);
	

	return UPDATE_CONTINUE;
}

void ModulePlayer::OnCollision(Collider* other) {
	if (other->type == NO_DMG_ENEMY) {
		fall.Reset();
		App->audio->PlayFx(ouch);
		current_animation = &fall;
	}
	else {
		--lives;
		App->audio->PlayFx(aaargh);
		hit = true;
		deathStartingPosY = position.y;
		collider->active = false;
		App->time->SetTimeScale(0.0f);
		if (lives <= 0) {
			collider->to_delete = true;
			destroyed = true;
		}
	}
}

void ModulePlayer::SetLives(const int& amount) {
	lives = amount;
}

int ModulePlayer::GetLives() const {
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
	//I dont put it on deltaTime base, cause i want the floor to change when the player "die"
	if (actualHorizonPercentage < temp- HORIZON_Y_SPEED) {
		actualHorizonPercentage += HORIZON_Y_SPEED;
	}
	else if (actualHorizonPercentage > temp) {
		actualHorizonPercentage -= HORIZON_Y_SPEED;
	}
	App->floor->SetHorizonYPerccentual(actualHorizonPercentage);
}

void ModulePlayer::CalculateSpeed() {
	float temp = (position.x +SCREEN_WIDTH/2) / SCREEN_WIDTH;
	speedStage = MIN_X_SPEED + (temp*(MAX_X_SPEED - MIN_X_SPEED));
}

fPoint ModulePlayer::GetRelativeWorldPosition() const {
	fPoint p;
	p.x= (position.x*2) / (SCREEN_WIDTH);
	p.y = (((position.y) *2) / (SCREEN_HEIGHT- current_animation->GetCurrentFrame().h))-1;
	p.z = 0.0f;
	return p;
}

fPoint ModulePlayer::GetPlayerCenterPos()const {
	return { position.x,position.y + current_animation->GetCurrentFrame().h/2,position.z };
}

void ModulePlayer::Death() {
	current_animation = &death;
	if (!deathBounced) {
		position.y += FALL_SPEED/2;
		if (abs(deathStartingPosY - position.y) > FALL_BOUNCE) {
			deathBounced = true;
		}
	}
	if (deathBounced && position.y > 0) {
		position.y -= FALL_SPEED;
	}
	if (current_animation->Finished()) {
		if (destroyed == true) {
			App->scene->End();
		}
		else
		{
			App->audio->PlayFx(getReady);
			App->time->SetTimeScale(1.0f);
		}
		death.Reset();
		current_animation = &run;
		deathBounced = false;
		recoverTimer = 0;
		hit = false;
	}
}

void ModulePlayer::Win() {
	win = true;
	collider->active = false;
	initialPosition = position;
	currentLerpPercentaje = 0.0f;
}

void ModulePlayer::AnimWin() {
	if (centered) {
		position.z += 5.0f*App->time->GetDeltaTime();

		float screenY = App->floor->GetFloorPositionFromZ(position.z);

		float scale = 1 - (screenY / App->floor->horizon.y);

		App->shadows->DrawShadow(position.x, screenY, scale);

		screenY += position.y*scale;

		if (position.z >= MAX_Z) {
			App->ui->TheEnd();
		}

		resizeStruct resizeInfo = { (int)(current_animation->GetCurrentFrame().w *scale),(int)(current_animation->GetCurrentFrame().h *scale) };
		App->renderer->AddToBlitBuffer(graphics, position.x, screenY, position.z, &(current_animation->GetCurrentFrame()), &resizeInfo);
	}
	else
	{
		currentLerpPercentaje += 0.05f;
		fPoint zero = { 0,0,0 };
		fPoint temp = zero - initialPosition;
		position.x = initialPosition.x + (temp.x*currentLerpPercentaje);
		position.y = initialPosition.y + (temp.y*currentLerpPercentaje);
		if (position.y > 0) {
			VerifyFlyAnimation();
		}
		else
		{
			current_animation = &run;
			VerifyFloorSpeed();
		}

		if (currentLerpPercentaje >= 1.0f) {
			centered = true;
			position.z = 0;
			current_animation = &run;
		}

		App->renderer->AddToBlitBuffer(graphics, position.x, position.y, position.z, &(current_animation->GetCurrentFrame()), nullptr);
	}
}

void ModulePlayer::AnimWelcome() {
	if (welcomeTimer >= WELCOME_RUN_TIME) {
		float tempSpeed = ((SCREEN_HEIGHT / 2) - (center.GetCurrentFrame().h / 2)) / WELCOME_TRANSITION_TIME;
		position.y += tempSpeed*App->time->GetDeltaTime();
		current_animation = &center;
		if (welcomeTimer >= WELCOME_RUN_TIME + WELCOME_TRANSITION_TIME) {
			welcome = true;
		}
	}
	welcomeTimer += App->time->GetDeltaTime();


	App->renderer->AddToBlitBuffer(graphics, position.x, position.y, position.z, &(current_animation->GetCurrentFrame()), nullptr);
}