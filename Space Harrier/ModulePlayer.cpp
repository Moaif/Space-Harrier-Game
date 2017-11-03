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
	run.speed = 0.1f;

	center.frames.push_back({ 108,2,26,49 });

	left1.frames.push_back({ 142,2,22,50 });

	left2.frames.push_back({ 170,3,20,48});

	right1.frames.push_back({ 197,3,20,48});
	right2.frames.push_back({ 221,3,22,49});

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
	position.x = 150;
	position.y = 120;
	position.z = 0;
	//Collider
	collider = App->collision->AddCollider({ 150,120,25,50 }, PLAYER, this);

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
	int speed = 1;

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		position.x -= speed;
		if (current_animation != &run) {
			VerifyFlyAnimation();
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		position.x += speed;
		if (current_animation != &run) {
			VerifyFlyAnimation();
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		position.y += speed;//TODO se pasara a run al colisionar con el terreno
	}

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		position.y -= speed;
		if (current_animation == &run) {
			VerifyFlyAnimation();
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		// TODO 6: Shoot a laser using the particle system
		float x =(float) (position.x + current_animation->GetCurrentFrame().w / 2) - (App->particles->laser.anim.GetCurrentFrame().w / 2);
		float y =(float) (position.y + current_animation->GetCurrentFrame().h / 2) - (App->particles->laser.anim.GetCurrentFrame().h / 2);
		App->particles->AddParticle(App->particles->laser, x, y);
	}

	// Draw everything --------------------------------------
	if (destroyed == false) {
		collider->rect.x = position.x;
		collider->rect.y = position.y;
		App->renderer->Blit(graphics, position.x, position.y, position.z, &(current_animation->GetCurrentFrame()));
	}

	return UPDATE_CONTINUE;
}

void ModulePlayer::OnCollision(Collider * col,Collider* other) {
	if (collider == col) {
		collider->to_delete = true;
		destroyed = true;
	}
}

void ModulePlayer::VerifyFlyAnimation() {

	if ((position.x + current_animation->GetCurrentFrame().w / 2) <= (SCREEN_WIDTH / 5)) {
		current_animation = &left2;
		return;
	}
	if ((position.x + current_animation->GetCurrentFrame().w / 2) <= (SCREEN_WIDTH / 5*2)) {
		current_animation = &left1;
		return;
	}
	if ((position.x + current_animation->GetCurrentFrame().w / 2) <= (SCREEN_WIDTH / 5*3)) {
		current_animation = &center;
		return;
	}
	if ((position.x + current_animation->GetCurrentFrame().w / 2) <= (SCREEN_WIDTH / 5*4)) {
		current_animation = &right1;
		return;
	}
	if ((position.x + current_animation->GetCurrentFrame().w / 2) <= SCREEN_WIDTH) {
		current_animation = &right2;
		return;
	}
}