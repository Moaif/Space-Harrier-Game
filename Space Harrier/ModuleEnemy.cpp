#include "Application.h"
#include "ModuleEnemy.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"
#include "ModuleAudio.h"
#include "Obstacle.h"
#include "Jellyfish.h"
#include "Drone.h"
#include "Fly.h"
#include "Dragon3.h"
#include "BodyPart.h"
#include "MetalBall.h"
#include "Dragon1.h"



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
	trees3 = App->textures->Load("assets/Trees3.png");
	rocks = App->textures->Load("assets/models.png");
	metalEnemy = App->textures->Load("assets/Metalball.png");
	dragonTexture = App->textures->Load("assets/Dragon3.png");
	dragonTexture2 = App->textures->Load("assets/Dragon1.png");

	droneStartSFX = App->audio->LoadFx("assets/music/SFX/DroneStart.wav");
	jellyStartSFX = App->audio->LoadFx("assets/music/SFX/JellyStart.wav");
	flyStartSFX = App->audio->LoadFx("assets/music/SFX/FlyStart.wav");
	metalStartSFX = App->audio->LoadFx("assets/music/SFX/MetalBall.wav");

	Enemy* tree = new Obstacle(trees);
	tree->anim.frames.push_back({ 206,48,44,163 });
	tree->shadow = false;
	enemies["tree1"] = tree;

	Enemy* tree3 = new Obstacle(trees3);
	tree3->anim.frames.push_back({ 0,0,79,184 });
	tree3->anim.frames.push_back({80,0,79,184});
	tree3->anim.frames.push_back({ 162,0,79,184 });
	tree3->anim.randFrame = true;
	tree3->anim.speed = 0;
	tree3->destructible = false;
	tree3->shadow = false;
	enemies["tree3"] = tree3;
	
	Enemy* tree3_2 = new Obstacle(trees3);
	tree3_2->anim.frames.push_back({ 242,104,108,82 });
	tree3_2->anim.frames.push_back({ 352,104,108,82 });
	tree3_2->anim.frames.push_back({ 463,104,108,82 });
	tree3_2->anim.randFrame = true;
	tree3_2->anim.speed = 0;
	enemies["tree3_2"] = tree3_2;

	Enemy* tree3_3 = new Obstacle(trees3);
	tree3_3->anim.frames.push_back({6,197,119,72});
	tree3_3->shadow = false;
	enemies["tree3_3"] = tree3_3;


	Enemy* rock = new Obstacle(rocks);
	rock->anim.frames.push_back({5,87,136,93});
	enemies["rock1"] = rock;

	Enemy* bush = new Obstacle(rocks);
	bush->anim.frames.push_back({ 4,5,126,71 });
	bush->noDmg = true;
	bush->shadow = false;
	enemies["bush1"] = bush;

	Enemy* bush2 = new Obstacle(rocks);
	bush2->anim.frames.push_back({ 153,2,183,83 });
	bush2->noDmg = true;
	bush2->shadow = false;
	enemies["bush2"] = bush2;

	Enemy* jellyfish = new Jellyfish(200.0f,enemiesTexture);
	jellyfish->anim.frames.push_back({0,43,80,90});
	jellyfish->anim.frames.push_back({ 83,43,80,90 });
	jellyfish->anim.frames.push_back({ 170,43,80,90 });
	jellyfish->anim.speed = 3.0f;
	jellyfish->speed = { 30.0f, 200.0f, 0.0f };
	jellyfish->efxIndex = jellyStartSFX;
	enemies["jelly1"] = jellyfish;

	Enemy* jellyfish2 = new Jellyfish(200.0f, enemiesTexture);
	jellyfish2->anim.frames.push_back({ 0,43,80,90 });
	jellyfish2->anim.frames.push_back({ 83,43,80,90 });
	jellyfish2->anim.frames.push_back({ 170,43,80,90 });
	jellyfish2->anim.speed = 3.0f;
	jellyfish2->speed = { -30.0f, 200.0f, 0.0f };
	jellyfish2->efxIndex = jellyStartSFX;
	enemies["jelly2"] = jellyfish2;

	Enemy* jellyfish3 = new Jellyfish(200.0f, enemiesTexture);
	jellyfish3->anim.frames.push_back({ 0,43,80,90 });
	jellyfish3->anim.frames.push_back({ 83,43,80,90 });
	jellyfish3->anim.frames.push_back({ 170,43,80,90 });
	jellyfish3->anim.speed = 3.0f;
	jellyfish3->speed = { 30.0f, 100.0f, 0.0f };
	jellyfish3->efxIndex = jellyStartSFX;
	enemies["jelly3"] = jellyfish3;

	Enemy* jellyfish4 = new Jellyfish(200.0f, enemiesTexture);
	jellyfish4->anim.frames.push_back({ 0,43,80,90 });
	jellyfish4->anim.frames.push_back({ 83,43,80,90 });
	jellyfish4->anim.frames.push_back({ 170,43,80,90 });
	jellyfish4->anim.speed = 3.0f;
	jellyfish4->speed = { -30.0f, 100.0f, 0.0f };
	jellyfish4->efxIndex = jellyStartSFX;
	enemies["jelly4"] = jellyfish4;

	Enemy* metalBall = new MetalBall(0,metalEnemy);
	metalBall->anim.frames.push_back({ 3, 2, 44, 44 });
	metalBall->anim.frames.push_back({ 53, 2, 44, 46 });
	metalBall->anim.frames.push_back({ 104, 2, 44, 46 });
	metalBall->anim.frames.push_back({ 1, 53, 56, 56 });
	metalBall->anim.frames.push_back({ 64, 53, 56, 56 });
	metalBall->anim.frames.push_back({ 1, 116, 64, 68 });
	metalBall->anim.frames.push_back({ 78, 116, 64, 68 });
	metalBall->anim.speed = 8.0f;
	metalBall->speed = {-80.0f,-200.0f,-5.0f};
	metalBall->efxIndex = metalStartSFX;
	enemies["metalball1"] = metalBall;

	Enemy* metalBall1 = new MetalBall(0, metalEnemy);
	metalBall1->anim.frames.push_back({ 3, 2, 44, 44 });
	metalBall1->anim.frames.push_back({ 53, 2, 44, 46 });
	metalBall1->anim.frames.push_back({ 104, 2, 44, 46 });
	metalBall1->anim.frames.push_back({ 1, 53, 56, 56 });
	metalBall1->anim.frames.push_back({ 64, 53, 56, 56 });
	metalBall1->anim.frames.push_back({ 1, 116, 64, 68 });
	metalBall1->anim.frames.push_back({ 78, 116, 64, 68 });
	metalBall1->anim.speed = 8.0f;
	metalBall1->speed = { 0.0f,200.0f,-5.0f };
	metalBall1->efxIndex = metalStartSFX;
	enemies["metalball2"] = metalBall1;

	Enemy* metalBall2 = new MetalBall(0, metalEnemy);
	metalBall2->anim.frames.push_back({ 3, 2, 44, 44 });
	metalBall2->anim.frames.push_back({ 53, 2, 44, 46 });
	metalBall2->anim.frames.push_back({ 104, 2, 44, 46 });
	metalBall2->anim.frames.push_back({ 1, 53, 56, 56 });
	metalBall2->anim.frames.push_back({ 64, 53, 56, 56 });
	metalBall2->anim.frames.push_back({ 1, 116, 64, 68 });
	metalBall2->anim.frames.push_back({ 78, 116, 64, 68 });
	metalBall2->anim.speed = 8.0f;
	metalBall2->speed = { 80.0f,-200.0f,-5.0f };
	metalBall2->efxIndex = metalStartSFX;
	enemies["metalball3"] = metalBall2;

	//Drone needs x=250 if comming from screen, or x=150 if comming from horizon
	Enemy* drone = new Drone(0, enemiesTexture);
	drone->anim.frames.push_back({ 0,7,80,34 });
	drone->anim.frames.push_back({ 84,7,80,34 });
	drone->anim.frames.push_back({ 168,7,80,34 });
	drone->anim.speed=3.0f;
	drone->speed = {100.0f,100.0f,5.0f };
	drone->efxIndex = droneStartSFX;
	enemies["drone0"] = drone;

	Enemy* drone1 = new Drone(0,enemiesTexture);
	drone1->anim.frames.push_back({0,7,80,34});
	drone1->anim.frames.push_back({ 84,7,80,34 });
	drone1->anim.frames.push_back({ 168,7,80,34 });
	drone1->anim.speed = 3.0f;
	drone1->speed = { -100.0f,100.0f,5.0f };
	drone1->efxIndex = droneStartSFX;
	enemies["drone1"] = drone1;

	Enemy* drone2 = new Drone(1, enemiesTexture);
	drone2->anim.frames.push_back({ 0,7,80,34 });
	drone2->anim.frames.push_back({ 84,7,80,34 });
	drone2->anim.frames.push_back({ 168,7,80,34 });
	drone2->anim.speed = 3.0f;
	drone2->speed = { 100.0f,0.0f,5.0f };
	drone2->efxIndex = droneStartSFX;
	enemies["drone2"] = drone2;

	Enemy* drone3 = new Drone(1, enemiesTexture);
	drone3->anim.frames.push_back({ 0,7,80,34 });
	drone3->anim.frames.push_back({ 84,7,80,34 });
	drone3->anim.frames.push_back({ 168,7,80,34 });
	drone3->anim.speed = 3.0f;
	drone3->speed = { -100.0f,0.0f,5.0f };
	drone3->efxIndex = droneStartSFX;
	enemies["drone3"] = drone3;

	Enemy* drone4 = new Drone(2, enemiesTexture);
	drone4->anim.frames.push_back({ 0,7,80,34 });
	drone4->anim.frames.push_back({ 84,7,80,34 });
	drone4->anim.frames.push_back({ 168,7,80,34 });
	drone4->anim.speed = 3.0f;
	drone4->speed = { 50.0f,120.0f,-5.0f };
	drone4->efxIndex = droneStartSFX;
	enemies["drone4"] = drone4;

	Enemy* drone5 = new Drone(2, enemiesTexture);
	drone5->anim.frames.push_back({ 0,7,80,34 });
	drone5->anim.frames.push_back({ 84,7,80,34 });
	drone5->anim.frames.push_back({ 168,7,80,34 });
	drone5->anim.speed = 3.0f;
	drone5->speed = { -50.0f,120.0f,-5.0f };
	drone5->efxIndex = droneStartSFX;
	enemies["drone5"] = drone5;

	Enemy* drone6 = new Drone(3, enemiesTexture);
	drone6->anim.frames.push_back({ 0,7,80,34 });
	drone6->anim.frames.push_back({ 84,7,80,34 });
	drone6->anim.frames.push_back({ 168,7,80,34 });
	drone6->anim.speed = 3.0f;
	drone6->speed = { 100.0f,100.0f,5.0f };
	drone6->efxIndex = droneStartSFX;
	enemies["drone6"] = drone6;

	Enemy* drone7 = new Drone(3, enemiesTexture);
	drone7->anim.frames.push_back({ 0,7,80,34 });
	drone7->anim.frames.push_back({ 84,7,80,34 });
	drone7->anim.frames.push_back({ 168,7,80,34 });
	drone7->anim.speed = 3.0f;
	drone7->speed = { -100.0f,100.0f,5.0f };
	drone7->efxIndex = droneStartSFX;
	enemies["drone7"] = drone7;

	Enemy* drone8 = new Drone(4,enemiesTexture);
	drone8->anim.frames.push_back({ 0,7,80,34 });
	drone8->anim.frames.push_back({ 84,7,80,34 });
	drone8->anim.frames.push_back({ 168,7,80,34 });
	drone8->anim.speed = 3.0f;
	drone8->speed = { 110.0f,-350.0f,-5.0f };
	drone8->efxIndex = droneStartSFX;
	enemies["drone8"] = drone8;

	Enemy* drone9 = new Drone(5, enemiesTexture);
	drone9->anim.frames.push_back({ 0,7,80,34 });
	drone9->anim.frames.push_back({ 84,7,80,34 });
	drone9->anim.frames.push_back({ 168,7,80,34 });
	drone9->anim.speed = 3.0f;
	drone9->speed = { 25.0f,-50.0f,-5.0f };
	drone9->efxIndex = droneStartSFX;
	enemies["drone9"] = drone9;

	Enemy* drone10 = new Drone(5, enemiesTexture);
	drone10->anim.frames.push_back({ 0,7,80,34 });
	drone10->anim.frames.push_back({ 84,7,80,34 });
	drone10->anim.frames.push_back({ 168,7,80,34 });
	drone10->anim.speed = 3.0f;
	drone10->speed = { 0.0f,-50.0f,-5.0f };
	drone10->efxIndex = droneStartSFX;
	enemies["drone10"] = drone10;

	Enemy* drone11 = new Drone(5, enemiesTexture);
	drone11->anim.frames.push_back({ 0,7,80,34 });
	drone11->anim.frames.push_back({ 84,7,80,34 });
	drone11->anim.frames.push_back({ 168,7,80,34 });
	drone11->anim.speed = 3.0f;
	drone11->speed = { -25.0f,-50.0f,-5.0f };
	drone11->efxIndex = droneStartSFX;
	enemies["drone11"] = drone11;

	Enemy* drone12 = new Drone(6, enemiesTexture);
	drone12->anim.frames.push_back({ 0,7,80,34 });
	drone12->anim.frames.push_back({ 84,7,80,34 });
	drone12->anim.frames.push_back({ 168,7,80,34 });
	drone12->speed = { 100.0f,0.0f,5.0f };
	drone12->efxIndex = droneStartSFX;
	enemies["drone12"] = drone12;

	Enemy* drone13 = new Drone(6, enemiesTexture);
	drone13->anim.frames.push_back({ 0,7,80,34 });
	drone13->anim.frames.push_back({ 84,7,80,34 });
	drone13->anim.frames.push_back({ 168,7,80,34 });
	drone13->anim.speed = 3.0f;
	drone13->speed = { -100.0f,0.0f,5.0f };
	drone13->efxIndex = droneStartSFX;
	enemies["drone13"] = drone13;

	Enemy* drone14 = new Drone(7, enemiesTexture);
	drone14->anim.frames.push_back({ 0,7,80,34 });
	drone14->anim.frames.push_back({ 84,7,80,34 });
	drone14->anim.frames.push_back({ 168,7,80,34 });
	drone14->anim.speed = 3.0f;
	drone14->speed = { 120.0f,75.0f,7.5f };
	drone14->efxIndex = droneStartSFX;
	enemies["drone14"] = drone14;

	Enemy* drone15 = new Drone(8, enemiesTexture);
	drone15->anim.frames.push_back({ 0,7,80,34 });
	drone15->anim.frames.push_back({ 84,7,80,34 });
	drone15->anim.frames.push_back({ 168,7,80,34 });
	drone15->anim.speed = 3.0f;
	drone15->speed = { 180.0f,25.0f,7.5f };
	drone15->efxIndex = droneStartSFX;
	enemies["drone15"] = drone15;

	Enemy* drone16 = new Drone(8, enemiesTexture);
	drone16->anim.frames.push_back({ 0,7,80,34 });
	drone16->anim.frames.push_back({ 84,7,80,34 });
	drone16->anim.frames.push_back({ 168,7,80,34 });
	drone16->anim.speed = 3.0f;
	drone16->speed = { -180.0f,25.0f,7.5f };
	drone16->efxIndex = droneStartSFX;
	enemies["drone16"] = drone16;

	//Fly needs x=150 or x=0 
	Enemy* fly = new Fly(0,enemiesTexture);
	fly->anim.frames.push_back({176,144,169,68});
	fly->speed = {50.0f, -140.0f, -5.0f};
	fly->efxIndex = flyStartSFX;
	enemies["fly1"] = fly;

	Enemy* fly2 = new Fly(0, enemiesTexture);
	fly2->anim.frames.push_back({ 176,144,169,68 });
	fly2->speed = { 0.0f, -140.0f, -5.0f };
	fly2->efxIndex = flyStartSFX;
	enemies["fly2"] = fly2;

	Enemy* fly3 = new Fly(0, enemiesTexture);
	fly3->anim.frames.push_back({ 176,144,169,68 });
	fly3->speed = { -50.0f, -140.0f, -5.0f };
	fly3->efxIndex = flyStartSFX;
	enemies["fly3"] = fly3;

	Enemy* fly4 = new Fly(1, enemiesTexture);
	fly4->anim.frames.push_back({ 176,144,169,68 });
	fly4->speed = { 70.0f, -120.0f, -5.0f };
	fly4->efxIndex = flyStartSFX;
	enemies["fly4"] = fly4;

	Enemy* fly5 = new Fly(1, enemiesTexture);
	fly5->anim.frames.push_back({ 176,144,169,68 });
	fly5->speed = { -70.0f, -120.0f, -5.0f };
	fly5->efxIndex = flyStartSFX;
	enemies["fly5"] = fly5;

	Enemy* dragon1 = new Dragon1();
	dragon1->speed = {80.0f,80.0f,-5.0f};
	dragon1->hits = 6;
	Enemy* head1 = new BodyPart(dragonTexture2,dragon1);
	head1->anim.frames.push_back({79,7,67,105});
	head1->anim.frames.push_back({0,7,71,108});
	head1->anim.frames.push_back({ 79,123,67,105 });
	head1->anim.frames.push_back({ 0,123,71,108 });
	head1->anim.frames.push_back({ 79,7,67,105 });
	head1->anim.frames.push_back({ 0,7,71,108 });
	head1->anim.frames.push_back({ 79,239,67,105 });
	head1->anim.frames.push_back({ 0,239,71,108 });
	head1->anim.frames.push_back({ 79,7,67,105 });
	head1->anim.frames.push_back({ 0,7,71,108 });
	head1->anim.frames.push_back({ 79,355,67,105 });
	head1->anim.frames.push_back({ 0,355,71,108 });
	head1->anim.speed = 0;
	Enemy* body1_1 = new BodyPart(dragonTexture2,dragon1);
	body1_1->anim.frames.push_back({161,26,114,73});
	body1_1->anim.frames.push_back({ 161,142,114,73 });
	body1_1->anim.frames.push_back({ 161,26,114,73 });
	body1_1->anim.frames.push_back({ 161,258,114,73 });
	body1_1->anim.frames.push_back({ 161,26,114,73 });
	body1_1->anim.frames.push_back({ 161,374,114,73 });
	body1_1->anim.speed = 0;
	body1_1->destructible = false;
	Enemy* body1_2 = new BodyPart(dragonTexture2, dragon1);
	body1_2->anim.frames.push_back({ 161,26,114,73 });
	body1_2->anim.frames.push_back({ 161,142,114,73 });
	body1_2->anim.frames.push_back({ 161,26,114,73 });
	body1_2->anim.frames.push_back({ 161,258,114,73 });
	body1_2->anim.frames.push_back({ 161,26,114,73 });
	body1_2->anim.frames.push_back({ 161,374,114,73 });
	body1_2->anim.speed = 0;
	body1_2->destructible = false;
	Enemy* body1_3 = new BodyPart(dragonTexture2, dragon1);
	body1_3->anim.frames.push_back({ 161,26,114,73 });
	body1_3->anim.frames.push_back({ 161,142,114,73 });
	body1_3->anim.frames.push_back({ 161,26,114,73 });
	body1_3->anim.frames.push_back({ 161,258,114,73 });
	body1_3->anim.frames.push_back({ 161,26,114,73 });
	body1_3->anim.frames.push_back({ 161,374,114,73 });
	body1_3->anim.speed = 0;
	body1_3->destructible = false;
	Enemy* body1_4 = new BodyPart(dragonTexture2, dragon1);
	body1_4->anim.frames.push_back({ 161,26,114,73 });
	body1_4->anim.frames.push_back({ 161,142,114,73 });
	body1_4->anim.frames.push_back({ 161,26,114,73 });
	body1_4->anim.frames.push_back({ 161,258,114,73 });
	body1_4->anim.frames.push_back({ 161,26,114,73 });
	body1_4->anim.frames.push_back({ 161,374,114,73 });
	body1_4->anim.speed = 0;
	body1_4->destructible = false;
	Enemy* body1_5 = new BodyPart(dragonTexture2, dragon1);
	body1_5->anim.frames.push_back({ 161,26,114,73 });
	body1_5->anim.frames.push_back({ 161,142,114,73 });
	body1_5->anim.frames.push_back({ 161,26,114,73 });
	body1_5->anim.frames.push_back({ 161,258,114,73 });
	body1_5->anim.frames.push_back({ 161,26,114,73 });
	body1_5->anim.frames.push_back({ 161,374,114,73 });
	body1_5->anim.speed = 0;
	body1_5->destructible = false;
	Enemy* body1_6 = new BodyPart(dragonTexture2, dragon1);
	body1_6->anim.frames.push_back({ 161,26,114,73 });
	body1_6->anim.frames.push_back({ 161,142,114,73 });
	body1_6->anim.frames.push_back({ 161,26,114,73 });
	body1_6->anim.frames.push_back({ 161,258,114,73 });
	body1_6->anim.frames.push_back({ 161,26,114,73 });
	body1_6->anim.frames.push_back({ 161,374,114,73 });
	body1_6->anim.speed = 0;
	body1_6->destructible = false;
	Enemy* body1_7 = new BodyPart(dragonTexture2, dragon1);
	body1_7->anim.frames.push_back({ 161,26,114,73 });
	body1_7->anim.frames.push_back({ 161,142,114,73 });
	body1_7->anim.frames.push_back({ 161,26,114,73 });
	body1_7->anim.frames.push_back({ 161,258,114,73 });
	body1_7->anim.frames.push_back({ 161,26,114,73 });
	body1_7->anim.frames.push_back({ 161,374,114,73 });
	body1_7->anim.speed = 0;
	body1_7->destructible = false;
	Enemy* tail1 = new BodyPart(dragonTexture2,dragon1);
	tail1->anim.frames.push_back({284,18,96,91});
	tail1->anim.frames.push_back({ 284,134,96,91 });
	tail1->anim.frames.push_back({ 284,18,96,91 });
	tail1->anim.frames.push_back({ 284,250,96,91 });
	tail1->anim.frames.push_back({ 284,18,96,91 });
	tail1->anim.frames.push_back({ 284,366,96,91 });
	tail1->anim.speed = 0;
	tail1->destructible = false;
	enemies["dragon1"] = dragon1;
	prototypePartsClearList.push_back(head1);
	prototypePartsClearList.push_back(body1_1);
	prototypePartsClearList.push_back(body1_2);
	prototypePartsClearList.push_back(body1_3);
	prototypePartsClearList.push_back(body1_4);
	prototypePartsClearList.push_back(body1_5);
	prototypePartsClearList.push_back(body1_6);
	prototypePartsClearList.push_back(body1_7);
	prototypePartsClearList.push_back(tail1);


	Enemy* dragon3_1 = new Dragon3(0);
	dragon3_1->speed = {-20,25,-4};
	dragon3_1->hits = 7;
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

	Enemy* dragon3_2 = new Dragon3(1);
	dragon3_2->speed = { 20,-25,-4 };
	dragon3_2->hits = 7;
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
	ASSERT(enemiesTexture != nullptr,AT("Enemies failed loading textures"));
	App->textures->Unload(trees);
	ASSERT(trees != nullptr, AT("Enemies failed loading textures"));
	App->textures->Unload(trees3);
	ASSERT(trees3 != nullptr, AT("Enemies failed loading textures"));
	App->textures->Unload(rocks);
	ASSERT(rocks != nullptr, AT("Enemies failed loading textures"));
	App->textures->Unload(metalEnemy);
	ASSERT(metalEnemy != nullptr, AT("Enemies failed loading textures"));
	App->textures->Unload(dragonTexture);
	ASSERT(dragonTexture != nullptr, AT("Enemies failed loading textures"));
	App->textures->Unload(dragonTexture2);
	ASSERT(dragonTexture2 != nullptr, AT("Enemies failed loading textures"));

	for (list<Enemy*>::iterator it = active.begin(); it != active.end(); ++it) {
		RELEASE(*it);
	}
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
		p->anim.Update();
		if (p->texture == nullptr) {
			for (vector<Enemy*>::iterator it2 = p->childs.begin(); it2 != p->childs.end(); ++it2) {
				resizeStruct resizeInfo = { (*it2)->screenPoint.w,(*it2)->screenPoint.h };
				App->renderer->AddToBlitBuffer((*it2)->texture,(float)(*it2)->screenPoint.x,(float)(*it2)->screenPoint.y,(*it2)->position.z,&((*it2)->anim.GetCurrentFrame()),&resizeInfo);
			}

		}
		else {
			if (p->screenPoint.h == 0 && p->screenPoint.w == 0) {
				App->renderer->AddToBlitBuffer(p->texture, p->position.x, p->position.y, p->position.z, &(p->anim.GetCurrentFrame()), nullptr);
			}
			else {
				resizeStruct resizeInfo = { p->screenPoint.w,p->screenPoint.h };
				App->renderer->AddToBlitBuffer(p->texture, (float)p->screenPoint.x, (float)p->screenPoint.y, p->position.z, &(p->anim.GetCurrentFrame()), &resizeInfo);
			}

			if (p->firstSound && p->efxIndex != 0) {
				App->audio->PlayFx(p->efxIndex);
				p->firstSound = false;
			}
		}
	}
	return UPDATE_CONTINUE;
}

void ModuleEnemy::AddEnemy(const Enemy& enemy, const float& x, const float& y, const float& z,Enemy* father)
{
	Enemy* p = enemy.Copy(x, y, z,father);
	ASSERT(p != nullptr,AT("Failed on enemy creation"));
	active.push_back(p);
	if (p->childs.size() > 0) {
		for (unsigned int i = 0; i < p->childs.size(); ++i) {
			activeBodyParts.push_back(p->childs[i]);
		}
	}
}


const Enemy* ModuleEnemy::GetById(string id) const {
	map<string, Enemy*>::const_iterator it = enemies.find(id);
	if (it != enemies.end()) {
		return (*it).second;
	}
	return nullptr;
}