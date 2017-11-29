#include "Application.h"
#include "ModuleEnemy.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"
#include "Obstacle.h"
#include "Jellyfish.h"
#include "Drone.h"
#include "Fly.h"
#include "Dragon3.h"
#include "BodyPart.h"



ModuleEnemy::ModuleEnemy()
{
}

ModuleEnemy::~ModuleEnemy()
{}

// Load assets
bool ModuleEnemy::Start()
{
	LOG("Loading Enemies");
	enemiesTexture = App->textures->Load("assets/Enemies.png");
	trees = App->textures->Load("assets/Arboles.png");
	rocks = App->textures->Load("assets/models.png");
	dragonTexture = App->textures->Load("assets/Dragon3.png");


	Enemy* tree = new Obstacle(trees);
	tree->anim.frames.push_back({ 206,48,44,163 });
	tree->shadow = false;
	enemies["tree1"] = tree;
	

	Enemy* rock = new Obstacle(rocks);
	rock->anim.frames.push_back({191,71,61,39});
	enemies["rock1"] = rock;

	Enemy* jellyfish = new Jellyfish(200.0f,enemiesTexture);
	jellyfish->anim.frames.push_back({0,43,80,90});
	jellyfish->anim.frames.push_back({ 83,43,80,90 });
	jellyfish->anim.frames.push_back({ 170,43,80,90 });
	jellyfish->speed = { 20.0f, 200.0f, 0.0f };
	enemies["jelly1"] = jellyfish;

	Enemy* jellyfish2 = new Jellyfish(200.0f, enemiesTexture);
	jellyfish2->anim.frames.push_back({ 0,43,80,90 });
	jellyfish2->anim.frames.push_back({ 83,43,80,90 });
	jellyfish2->anim.frames.push_back({ 170,43,80,90 });
	jellyfish2->speed = { -20.0f, 200.0f, 0.0f };
	enemies["jelly2"] = jellyfish2;

	Enemy* jellyfish3 = new Jellyfish(200.0f, enemiesTexture);
	jellyfish3->anim.frames.push_back({ 0,43,80,90 });
	jellyfish3->anim.frames.push_back({ 83,43,80,90 });
	jellyfish3->anim.frames.push_back({ 170,43,80,90 });
	jellyfish3->speed = { 20.0f, 100.0f, 0.0f };
	enemies["jelly3"] = jellyfish3;

	Enemy* jellyfish4 = new Jellyfish(200.0f, enemiesTexture);
	jellyfish4->anim.frames.push_back({ 0,43,80,90 });
	jellyfish4->anim.frames.push_back({ 83,43,80,90 });
	jellyfish4->anim.frames.push_back({ 170,43,80,90 });
	jellyfish4->speed = { -20.0f, 100.0f, 0.0f };
	enemies["jelly4"] = jellyfish4;

	Enemy* drone = new Drone(0,enemiesTexture);
	drone->anim.frames.push_back({0,7,80,34});
	drone->anim.frames.push_back({ 84,7,80,34 });
	drone->anim.frames.push_back({ 168,7,80,34 });
	drone->speed = { 100.0f,200.0f,10.0f };
	enemies["drone1"] = drone;

	Enemy* drone2 = new Drone(1, enemiesTexture);
	drone2->anim.frames.push_back({ 0,7,80,34 });
	drone2->anim.frames.push_back({ 84,7,80,34 });
	drone2->anim.frames.push_back({ 168,7,80,34 });
	drone2->speed = { 120.0f,0.0f,10.0f };
	enemies["drone2"] = drone2;

	Enemy* drone3 = new Drone(1, enemiesTexture);
	drone3->anim.frames.push_back({ 0,7,80,34 });
	drone3->anim.frames.push_back({ 84,7,80,34 });
	drone3->anim.frames.push_back({ 168,7,80,34 });
	drone3->speed = { -120.0f,0.0f,10.0f };
	enemies["drone3"] = drone3;

	Enemy* drone4 = new Drone(2, enemiesTexture);
	drone4->anim.frames.push_back({ 0,7,80,34 });
	drone4->anim.frames.push_back({ 84,7,80,34 });
	drone4->anim.frames.push_back({ 168,7,80,34 });
	drone4->speed = { -50.0f,120.0f,-5.0f };
	enemies["drone4"] = drone4;

	Enemy* drone5 = new Drone(2, enemiesTexture);
	drone5->anim.frames.push_back({ 0,7,80,34 });
	drone5->anim.frames.push_back({ 84,7,80,34 });
	drone5->anim.frames.push_back({ 168,7,80,34 });
	drone5->speed = { 50.0f,120.0f,-5.0f };
	enemies["drone5"] = drone5;

	Enemy* drone6 = new Drone(3, enemiesTexture);
	drone6->anim.frames.push_back({ 0,7,80,34 });
	drone6->anim.frames.push_back({ 84,7,80,34 });
	drone6->anim.frames.push_back({ 168,7,80,34 });
	drone6->speed = { 100.0f,200.0f,10.0f };
	enemies["drone6"] = drone6;

	Enemy* drone7 = new Drone(3, enemiesTexture);
	drone7->anim.frames.push_back({ 0,7,80,34 });
	drone7->anim.frames.push_back({ 84,7,80,34 });
	drone7->anim.frames.push_back({ 168,7,80,34 });
	drone7->speed = { -100.0f,200.0f,10.0f };
	enemies["drone7"] = drone7;


	Enemy* fly = new Fly(0,enemiesTexture);
	fly->anim.frames.push_back({8,144,78,32});
	fly->speed = {25.0f, -70.0f, -2.5f};
	enemies["fly1"] = fly;

	Enemy* fly2 = new Fly(0, enemiesTexture);
	fly2->anim.frames.push_back({ 8,144,78,32 });
	fly2->speed = { 0.0f, -70.0f, -2.5f };
	enemies["fly2"] = fly2;

	Enemy* fly3 = new Fly(0, enemiesTexture);
	fly3->anim.frames.push_back({ 8,144,78,32 });
	fly3->speed = { -25.0f, -70.0f, -2.5f };
	enemies["fly3"] = fly3;

	Enemy* fly4 = new Fly(1, enemiesTexture);
	fly4->anim.frames.push_back({ 8,144,78,32 });
	fly4->speed = { 35.0f, -60.0f, -2.5f };
	enemies["fly4"] = fly4;

	Enemy* fly5 = new Fly(1, enemiesTexture);
	fly5->anim.frames.push_back({ 8,144,78,32 });
	fly5->speed = { -35.0f, -60.0f, -2.5f };
	enemies["fly5"] = fly5;


	Enemy* dragon3_1 = new Dragon3(0,nullptr);
	dragon3_1->speed = {-20,25,-4};
	dragon3_1->hits = 6;
	Enemy* head3_1 = new BodyPart(dragonTexture, dragon3_1);
	head3_1->anim.frames.push_back({ 1,5,91,115 });
	head3_1->anim.frames.push_back({ 1,127,91,115 });
	head3_1->anim.frames.push_back({ 1,249,91,115 });
	head3_1->anim.frames.push_back({ 1,127,91,115 });
	head3_1->anim.frames.push_back({ 218,124,91,115 });
	head3_1->anim.frames.push_back({ 1,127,91,115 });
	head3_1->anim.frames.push_back({ 222,250,91,115 });
	head3_1->anim.speed = 0;
	Enemy* tail3_1 = new BodyPart(dragonTexture, dragon3_1);
	tail3_1->anim.frames.push_back({ 207,14,96,91 });
	tail3_1->destructible = false;
	Enemy* body3_1_1 = new BodyPart(dragonTexture, dragon3_1);
	body3_1_1->anim.frames.push_back({102,10,93,67});
	body3_1_1->anim.frames.push_back({ 102,132,93,67 });
	body3_1_1->anim.frames.push_back({ 102,254,93,67 });
	body3_1_1->anim.frames.push_back({ 102,132,93,67 });
	body3_1_1->anim.frames.push_back({ 319,129,93,67 });
	body3_1_1->anim.frames.push_back({ 102,132,93,67 });
	body3_1_1->anim.frames.push_back({ 323,255,93,67 });
	body3_1_1->anim.speed = 0;
	body3_1_1->destructible = false;
	Enemy* body3_1_2 = new BodyPart(dragonTexture, dragon3_1);
	body3_1_2->anim.frames.push_back({ 102,10,93,67 });
	body3_1_2->anim.frames.push_back({ 102,132,93,67 });
	body3_1_2->anim.frames.push_back({ 102,254,93,67 });
	body3_1_2->anim.frames.push_back({ 102,132,93,67 });
	body3_1_2->anim.frames.push_back({ 319,129,93,67 });
	body3_1_2->anim.frames.push_back({ 102,132,93,67 });
	body3_1_2->anim.frames.push_back({ 323,255,93,67 });
	body3_1_2->anim.speed = 0;
	body3_1_2->destructible = false;
	Enemy* body3_1_3 = new BodyPart(dragonTexture, dragon3_1);
	body3_1_3->anim.frames.push_back({ 102,10,93,67 });
	body3_1_3->anim.frames.push_back({ 102,132,93,67 });
	body3_1_3->anim.frames.push_back({ 102,254,93,67 });
	body3_1_3->anim.frames.push_back({ 102,132,93,67 });
	body3_1_3->anim.frames.push_back({ 319,129,93,67 });
	body3_1_3->anim.frames.push_back({ 102,132,93,67 });
	body3_1_3->anim.frames.push_back({ 323,255,93,67 });
	body3_1_3->anim.speed = 0;
	body3_1_3->destructible = false;
	Enemy* body3_1_4 = new BodyPart(dragonTexture, dragon3_1);
	body3_1_4->anim.frames.push_back({ 102,10,93,67 });
	body3_1_4->anim.frames.push_back({ 102,132,93,67 });
	body3_1_4->anim.frames.push_back({ 102,254,93,67 });
	body3_1_4->anim.frames.push_back({ 102,132,93,67 });
	body3_1_4->anim.frames.push_back({ 319,129,93,67 });
	body3_1_4->anim.frames.push_back({ 102,132,93,67 });
	body3_1_4->anim.frames.push_back({ 323,255,93,67 });
	body3_1_4->anim.speed = 0;
	body3_1_4->destructible = false;
	Enemy* body3_1_5 = new BodyPart(dragonTexture, dragon3_1);
	body3_1_5->anim.frames.push_back({ 102,10,93,67 });
	body3_1_5->anim.frames.push_back({ 102,132,93,67 });
	body3_1_5->anim.frames.push_back({ 102,254,93,67 });
	body3_1_5->anim.frames.push_back({ 102,132,93,67 });
	body3_1_5->anim.frames.push_back({ 319,129,93,67 });
	body3_1_5->anim.frames.push_back({ 102,132,93,67 });
	body3_1_5->anim.frames.push_back({ 323,255,93,67 });
	body3_1_5->anim.speed = 0;
	body3_1_5->destructible = false;
	enemies["dragon3_1"] = dragon3_1;
	prototypePartsClearList.push_back(head3_1);
	prototypePartsClearList.push_back(tail3_1);
	prototypePartsClearList.push_back(body3_1_1);
	prototypePartsClearList.push_back(body3_1_2);
	prototypePartsClearList.push_back(body3_1_3);
	prototypePartsClearList.push_back(body3_1_4);
	prototypePartsClearList.push_back(body3_1_5);

	Enemy* dragon3_2 = new Dragon3(1,nullptr);
	dragon3_2->speed = { 20,-25,-4 };
	dragon3_2->hits = 6;
	Enemy* head3_2 = new BodyPart(dragonTexture, dragon3_2);
	head3_2->anim.frames.push_back({ 1,5,91,115 });
	head3_2->anim.frames.push_back({1,127,91,115});
	head3_2->anim.frames.push_back({ 1,249,91,115 });
	head3_2->anim.frames.push_back({ 1,127,91,115 });
	head3_2->anim.frames.push_back({ 218,124,91,115 });
	head3_2->anim.frames.push_back({ 1,127,91,115 });
	head3_2->anim.frames.push_back({ 222,250,91,115 });
	head3_2->anim.speed = 0;
	Enemy* tail3_2 = new BodyPart(dragonTexture, dragon3_2);
	tail3_2->anim.frames.push_back({ 207,14,96,91 });
	tail3_2->destructible = false;
	Enemy* body3_2_1 = new BodyPart(dragonTexture, dragon3_2);
	body3_2_1->anim.frames.push_back({ 102,10,93,67 });
	body3_2_1->anim.frames.push_back({ 102,132,93,67 });
	body3_2_1->anim.frames.push_back({ 102,254,93,67 });
	body3_2_1->anim.frames.push_back({ 102,132,93,67 });
	body3_2_1->anim.frames.push_back({ 319,129,93,67 });
	body3_2_1->anim.frames.push_back({ 102,132,93,67 });
	body3_2_1->anim.frames.push_back({ 323,255,93,67 });
	body3_2_1->anim.speed = 0;
	body3_2_1->destructible = false;
	Enemy* body3_2_2 = new BodyPart(dragonTexture, dragon3_2);
	body3_2_2->anim.frames.push_back({ 102,10,93,67 });
	body3_2_2->anim.frames.push_back({ 102,132,93,67 });
	body3_2_2->anim.frames.push_back({ 102,254,93,67 });
	body3_2_2->anim.frames.push_back({ 102,132,93,67 });
	body3_2_2->anim.frames.push_back({ 319,129,93,67 });
	body3_2_2->anim.frames.push_back({ 102,132,93,67 });
	body3_2_2->anim.frames.push_back({ 323,255,93,67 });
	body3_2_2->anim.speed = 0;
	body3_2_2->destructible = false;
	Enemy* body3_2_3 = new BodyPart(dragonTexture, dragon3_2);
	body3_2_3->anim.frames.push_back({ 102,10,93,67 });
	body3_2_3->anim.frames.push_back({ 102,132,93,67 });
	body3_2_3->anim.frames.push_back({ 102,254,93,67 });
	body3_2_3->anim.frames.push_back({ 102,132,93,67 });
	body3_2_3->anim.frames.push_back({ 319,129,93,67 });
	body3_2_3->anim.frames.push_back({ 102,132,93,67 });
	body3_2_3->anim.frames.push_back({ 323,255,93,67 });
	body3_2_3->anim.speed = 0;
	body3_2_3->destructible = false;
	Enemy* body3_2_4 = new BodyPart(dragonTexture, dragon3_2);
	body3_2_4->anim.frames.push_back({ 102,10,93,67 });
	body3_2_4->anim.frames.push_back({ 102,132,93,67 });
	body3_2_4->anim.frames.push_back({ 102,254,93,67 });
	body3_2_4->anim.frames.push_back({ 102,132,93,67 });
	body3_2_4->anim.frames.push_back({ 319,129,93,67 });
	body3_2_4->anim.frames.push_back({ 102,132,93,67 });
	body3_2_4->anim.frames.push_back({ 323,255,93,67 });
	body3_2_4->anim.speed = 0;
	body3_2_4->destructible = false;
	Enemy* body3_2_5 = new BodyPart(dragonTexture, dragon3_2);
	body3_2_5->anim.frames.push_back({ 102,10,93,67 });
	body3_2_5->anim.frames.push_back({ 102,132,93,67 });
	body3_2_5->anim.frames.push_back({ 102,254,93,67 });
	body3_2_5->anim.frames.push_back({ 102,132,93,67 });
	body3_2_5->anim.frames.push_back({ 319,129,93,67 });
	body3_2_5->anim.frames.push_back({ 102,132,93,67 });
	body3_2_5->anim.frames.push_back({ 323,255,93,67 });
	body3_2_5->anim.speed = 0;
	body3_2_5->destructible = false;
	enemies["dragon3_2"] = dragon3_2;
	prototypePartsClearList.push_back(head3_2);
	prototypePartsClearList.push_back(tail3_2);
	prototypePartsClearList.push_back(body3_2_1);
	prototypePartsClearList.push_back(body3_2_2);
	prototypePartsClearList.push_back(body3_2_3);
	prototypePartsClearList.push_back(body3_2_4);
	prototypePartsClearList.push_back(body3_2_5);



	return true;
}

// Unload assets
bool ModuleEnemy::CleanUp()
{
	LOG("Unloading enemies");
	App->textures->Unload(enemiesTexture);
	App->textures->Unload(trees);
	App->textures->Unload(rocks);
	App->textures->Unload(dragonTexture);

	for (list<Enemy*>::iterator it = active.begin(); it != active.end(); ++it)
		RELEASE(*it);

	active.clear();

	for (map<string, Enemy*>::iterator it = enemies.begin(); it != enemies.end(); ++it) {
		RELEASE((*it).second);
	}
	enemies.clear();

	for (list<Enemy*>::iterator it = prototypePartsClearList.begin(); it != prototypePartsClearList.end(); ++it) {
		RELEASE(*it);
	}
	prototypePartsClearList.clear();

	for (list<Enemy*>::iterator it = activeBodyParts.begin(); it != activeBodyParts.end(); ++it) {
		RELEASE(*it);
	}
	activeBodyParts.clear();

	return true;
}

// PreUpdate to clear up all dirty particles
update_status ModuleEnemy::PreUpdate()
{
	for (list<Enemy*>::iterator it = active.begin(); it != active.end();)
	{
		if ((*it)->to_delete == true)
		{
			RELEASE(*it);
			it = active.erase(it);
		}
		else
			++it;
	}
	for (list<Enemy*>::iterator it = activeBodyParts.begin(); it != activeBodyParts.end();) {
		if ((*it)->to_delete == true)
		{
			RELEASE(*it);
			it = activeBodyParts.erase(it);
		}
		else
			++it;
	}

	return UPDATE_CONTINUE;
}

// Update all enemies logic and draw them
update_status ModuleEnemy::Update()
{
	for (list<Enemy*>::iterator it = active.begin(); it != active.end(); ++it)
	{
		Enemy* p = *it;

		p->Update();
		if (p->texture == nullptr) {
			for (vector<Enemy*>::iterator it2 = p->childs.begin(); it2 != p->childs.end(); ++it2) {
				resizeStruct resizeInfo = { (*it2)->screenPoint.w,(*it2)->screenPoint.h };
				App->renderer->AddToBlitBuffer((*it2)->texture,(*it2)->screenPoint.x,(*it2)->screenPoint.y,(*it2)->position.z,&((*it2)->anim.GetCurrentFrame()),&resizeInfo);
			}
		}
		else if (p->screenPoint.h ==0 && p->screenPoint.w ==0) {
			App->renderer->AddToBlitBuffer(p->texture, p->position.x, p->position.y, p->position.z, &(p->anim.GetCurrentFrame()),nullptr);
		}
		else {
			resizeStruct resizeInfo = { p->screenPoint.w,p->screenPoint.h };
			App->renderer->AddToBlitBuffer(p->texture, (float)p->screenPoint.x, (float)p->screenPoint.y, p->position.z, &(p->anim.GetCurrentFrame()), &resizeInfo);
		}
	}
	return UPDATE_CONTINUE;
}

void ModuleEnemy::AddEnemy(const Enemy& enemy, float x, float y, float z,Enemy* father)
{
	Enemy* p = enemy.Copy(x, y, z,father);
	active.push_back(p);
	if (p->childs.size() > 0) {
		for (int i = 0; i < p->childs.size(); ++i) {
			activeBodyParts.push_back(p->childs[i]);
		}
	}
}


Enemy* ModuleEnemy::GetById(string id) {
	map<string, Enemy*>::iterator it = enemies.find(id);
	if (it != enemies.end()) {
		return enemies[id];
	}
	return nullptr;
}