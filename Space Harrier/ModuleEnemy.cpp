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
	graphics = App->textures->Load("assets/ModuleEnemies.png");
	ASSERT(graphics != nullptr,AT("Failed on loading enemies texture"));

	droneStartSFX = App->audio->LoadFx("assets/music/SFX/DroneStart.wav");
	jellyStartSFX = App->audio->LoadFx("assets/music/SFX/JellyStart.wav");
	flyStartSFX = App->audio->LoadFx("assets/music/SFX/FlyStart.wav");
	metalStartSFX = App->audio->LoadFx("assets/music/SFX/MetalBall.wav");

	Enemy* tree = new Obstacle(graphics);
	tree->anim.frames.push_back({ 440,440,42,160 });
	tree->shadow = false;
	enemies["tree1"] = tree;

	Enemy* tree3 = new Obstacle(graphics);
	tree3->anim.frames.push_back({ 750,5,77,184 });
	tree3->anim.frames.push_back({830,5,77,184});
	tree3->anim.frames.push_back({ 913,6,79,184 });
	tree3->anim.randFrame = true;
	tree3->anim.speed = 0;
	tree3->destructible = false;
	tree3->shadow = false;
	enemies["tree3"] = tree3;
	
	Enemy* tree3_2 = new Obstacle(graphics);
	tree3_2->anim.frames.push_back({ 991,109,108,81 });
	tree3_2->anim.frames.push_back({ 1101,111,106,81 });
	tree3_2->anim.frames.push_back({ 1212,111,108,81 });
	tree3_2->anim.randFrame = true;
	tree3_2->anim.speed = 0;
	enemies["tree3_2"] = tree3_2;

	Enemy* tree3_3 = new Obstacle(graphics);
	tree3_3->anim.frames.push_back({755,202,119,72});
	tree3_3->shadow = false;
	enemies["tree3_3"] = tree3_3;


	Enemy* rock = new Obstacle(graphics);
	rock->anim.frames.push_back({410,330,136,93});
	enemies["rock1"] = rock;

	Enemy* bush = new Obstacle(graphics);
	bush->anim.frames.push_back({ 409,248,126,71 });
	bush->noDmg = true;
	bush->shadow = false;
	enemies["bush1"] = bush;

	Enemy* bush2 = new Obstacle(graphics);
	bush2->anim.frames.push_back({ 558,245,183,83 });
	bush2->noDmg = true;
	bush2->shadow = false;
	enemies["bush2"] = bush2;

	Enemy* jellyfish = new Jellyfish(200.0f,graphics);
	jellyfish->anim.frames.push_back({397,45,80,90});
	jellyfish->anim.frames.push_back({ 480,45,80,90 });
	jellyfish->anim.frames.push_back({ 567,46,80,90 });
	jellyfish->anim.speed = 3.0f;
	jellyfish->speed = { 30.0f, 200.0f, 0.0f };
	jellyfish->efxIndex = jellyStartSFX;
	enemies["jelly1"] = jellyfish;

	Enemy* jellyfish2 = new Jellyfish(200.0f, graphics);
	jellyfish2->anim.frames.push_back({ 397,45,80,90 });
	jellyfish2->anim.frames.push_back({ 480,45,80,90 });
	jellyfish2->anim.frames.push_back({ 567,46,80,90 });
	jellyfish2->anim.speed = 3.0f;
	jellyfish2->speed = { -30.0f, 200.0f, 0.0f };
	jellyfish2->efxIndex = jellyStartSFX;
	enemies["jelly2"] = jellyfish2;

	Enemy* jellyfish3 = new Jellyfish(200.0f, graphics);
	jellyfish3->anim.frames.push_back({ 397,45,80,90 });
	jellyfish3->anim.frames.push_back({ 480,45,80,90 });
	jellyfish3->anim.frames.push_back({ 567,46,80,90 });
	jellyfish3->anim.speed = 3.0f;
	jellyfish3->speed = { 30.0f, 100.0f, 0.0f };
	jellyfish3->efxIndex = jellyStartSFX;
	enemies["jelly3"] = jellyfish3;

	Enemy* jellyfish4 = new Jellyfish(200.0f, graphics);
	jellyfish4->anim.frames.push_back({ 397,45,80,90 });
	jellyfish4->anim.frames.push_back({ 480,45,80,90 });
	jellyfish4->anim.frames.push_back({ 567,46,80,90 });
	jellyfish4->anim.speed = 3.0f;
	jellyfish4->speed = { -30.0f, 100.0f, 0.0f };
	jellyfish4->efxIndex = jellyStartSFX;
	enemies["jelly4"] = jellyfish4;

	Enemy* metalBall = new MetalBall(0,graphics);
	metalBall->anim.frames.push_back({ 18, 844, 46, 46 });
	metalBall->anim.frames.push_back({ 68, 844, 48, 48 });
	metalBall->anim.frames.push_back({ 119, 844, 48, 48 });
	metalBall->anim.frames.push_back({ 16, 895, 58, 58 });
	metalBall->anim.frames.push_back({ 79, 895, 58, 58 });
	metalBall->anim.frames.push_back({ 16, 958, 66, 70 });
	metalBall->anim.frames.push_back({ 93, 958, 66, 70 });
	metalBall->anim.speed = 8.0f;
	metalBall->speed = {-80.0f,-200.0f,-5.0f};
	metalBall->efxIndex = metalStartSFX;
	enemies["metalball1"] = metalBall;

	Enemy* metalBall1 = new MetalBall(0, graphics);
	metalBall1->anim.frames.push_back({ 18, 844, 46, 46 });
	metalBall1->anim.frames.push_back({ 68, 844, 48, 48 });
	metalBall1->anim.frames.push_back({ 119, 844, 48, 48 });
	metalBall1->anim.frames.push_back({ 16, 895, 58, 58 });
	metalBall1->anim.frames.push_back({ 79, 895, 58, 58 });
	metalBall1->anim.frames.push_back({ 16, 958, 66, 70 });
	metalBall1->anim.frames.push_back({ 93, 958, 66, 70 });
	metalBall1->anim.speed = 8.0f;
	metalBall1->speed = { 0.0f,200.0f,-5.0f };
	metalBall1->efxIndex = metalStartSFX;
	enemies["metalball2"] = metalBall1;

	Enemy* metalBall2 = new MetalBall(0, graphics);
	metalBall2->anim.frames.push_back({ 18, 844, 46, 46 });
	metalBall2->anim.frames.push_back({ 68, 844, 48, 48 });
	metalBall2->anim.frames.push_back({ 119, 844, 48, 48 });
	metalBall2->anim.frames.push_back({ 16, 895, 58, 58 });
	metalBall2->anim.frames.push_back({ 79, 895, 58, 58 });
	metalBall2->anim.frames.push_back({ 16, 958, 66, 70 });
	metalBall2->anim.frames.push_back({ 93, 958, 66, 70 });
	metalBall2->anim.speed = 8.0f;
	metalBall2->speed = { 80.0f,-200.0f,-5.0f };
	metalBall2->efxIndex = metalStartSFX;
	enemies["metalball3"] = metalBall2;

	//Drone needs x=250 if comming from screen, or x=150 if comming from horizon
	Enemy* drone = new Drone(0, graphics);
	drone->anim.frames.push_back({ 397,9,80,34 });
	drone->anim.frames.push_back({ 481,9,80,34 });
	drone->anim.frames.push_back({ 565,9,80,34 });
	drone->anim.speed=3.0f;
	drone->speed = {100.0f,100.0f,5.0f };
	drone->efxIndex = droneStartSFX;
	enemies["drone0"] = drone;

	Enemy* drone1 = new Drone(0,graphics);
	drone1->anim.frames.push_back({ 397,9,80,34 });
	drone1->anim.frames.push_back({ 481,9,80,34 });
	drone1->anim.frames.push_back({ 565,9,80,34 });
	drone1->anim.speed = 3.0f;
	drone1->speed = { -100.0f,100.0f,5.0f };
	drone1->efxIndex = droneStartSFX;
	enemies["drone1"] = drone1;

	Enemy* drone2 = new Drone(1, graphics);
	drone2->anim.frames.push_back({ 397,9,80,34 });
	drone2->anim.frames.push_back({ 481,9,80,34 });
	drone2->anim.frames.push_back({ 565,9,80,34 });
	drone2->anim.speed = 3.0f;
	drone2->speed = { 100.0f,0.0f,5.0f };
	drone2->efxIndex = droneStartSFX;
	enemies["drone2"] = drone2;

	Enemy* drone3 = new Drone(1, graphics);
	drone3->anim.frames.push_back({ 397,9,80,34 });
	drone3->anim.frames.push_back({ 481,9,80,34 });
	drone3->anim.frames.push_back({ 565,9,80,34 });
	drone3->anim.speed = 3.0f;
	drone3->speed = { -100.0f,0.0f,5.0f };
	drone3->efxIndex = droneStartSFX;
	enemies["drone3"] = drone3;

	Enemy* drone4 = new Drone(2, graphics);
	drone4->anim.frames.push_back({ 397,9,80,34 });
	drone4->anim.frames.push_back({ 481,9,80,34 });
	drone4->anim.frames.push_back({ 565,9,80,34 });
	drone4->anim.speed = 3.0f;
	drone4->speed = { 50.0f,120.0f,-5.0f };
	drone4->efxIndex = droneStartSFX;
	enemies["drone4"] = drone4;

	Enemy* drone5 = new Drone(2, graphics);
	drone5->anim.frames.push_back({ 397,9,80,34 });
	drone5->anim.frames.push_back({ 481,9,80,34 });
	drone5->anim.frames.push_back({ 565,9,80,34 });
	drone5->anim.speed = 3.0f;
	drone5->speed = { -50.0f,120.0f,-5.0f };
	drone5->efxIndex = droneStartSFX;
	enemies["drone5"] = drone5;

	Enemy* drone6 = new Drone(3, graphics);
	drone6->anim.frames.push_back({ 397,9,80,34 });
	drone6->anim.frames.push_back({ 481,9,80,34 });
	drone6->anim.frames.push_back({ 565,9,80,34 });
	drone6->anim.speed = 3.0f;
	drone6->speed = { 100.0f,100.0f,5.0f };
	drone6->efxIndex = droneStartSFX;
	enemies["drone6"] = drone6;

	Enemy* drone7 = new Drone(3, graphics);
	drone7->anim.frames.push_back({ 397,9,80,34 });
	drone7->anim.frames.push_back({ 481,9,80,34 });
	drone7->anim.frames.push_back({ 565,9,80,34 });
	drone7->anim.speed = 3.0f;
	drone7->speed = { -100.0f,100.0f,5.0f };
	drone7->efxIndex = droneStartSFX;
	enemies["drone7"] = drone7;

	Enemy* drone8 = new Drone(4,graphics);
	drone8->anim.frames.push_back({ 397,9,80,34 });
	drone8->anim.frames.push_back({ 481,9,80,34 });
	drone8->anim.frames.push_back({ 565,9,80,34 });
	drone8->anim.speed = 3.0f;
	drone8->speed = { 110.0f,-350.0f,-5.0f };
	drone8->efxIndex = droneStartSFX;
	enemies["drone8"] = drone8;

	Enemy* drone9 = new Drone(5, graphics);
	drone9->anim.frames.push_back({ 397,9,80,34 });
	drone9->anim.frames.push_back({ 481,9,80,34 });
	drone9->anim.frames.push_back({ 565,9,80,34 });
	drone9->anim.speed = 3.0f;
	drone9->speed = { 25.0f,-50.0f,-5.0f };
	drone9->efxIndex = droneStartSFX;
	enemies["drone9"] = drone9;

	Enemy* drone10 = new Drone(5, graphics);
	drone10->anim.frames.push_back({ 397,9,80,34 });
	drone10->anim.frames.push_back({ 481,9,80,34 });
	drone10->anim.frames.push_back({ 565,9,80,34 });
	drone10->anim.speed = 3.0f;
	drone10->speed = { 0.0f,-50.0f,-5.0f };
	drone10->efxIndex = droneStartSFX;
	enemies["drone10"] = drone10;

	Enemy* drone11 = new Drone(5, graphics);
	drone11->anim.frames.push_back({ 397,9,80,34 });
	drone11->anim.frames.push_back({ 481,9,80,34 });
	drone11->anim.frames.push_back({ 565,9,80,34 });
	drone11->anim.speed = 3.0f;
	drone11->speed = { -25.0f,-50.0f,-5.0f };
	drone11->efxIndex = droneStartSFX;
	enemies["drone11"] = drone11;

	Enemy* drone12 = new Drone(6, graphics);
	drone12->anim.frames.push_back({ 397,9,80,34 });
	drone12->anim.frames.push_back({ 481,9,80,34 });
	drone12->anim.frames.push_back({ 565,9,80,34 });
	drone12->speed = { 100.0f,0.0f,5.0f };
	drone12->efxIndex = droneStartSFX;
	enemies["drone12"] = drone12;

	Enemy* drone13 = new Drone(6, graphics);
	drone13->anim.frames.push_back({ 397,9,80,34 });
	drone13->anim.frames.push_back({ 481,9,80,34 });
	drone13->anim.frames.push_back({ 565,9,80,34 });
	drone13->anim.speed = 3.0f;
	drone13->speed = { -100.0f,0.0f,5.0f };
	drone13->efxIndex = droneStartSFX;
	enemies["drone13"] = drone13;

	Enemy* drone14 = new Drone(7, graphics);
	drone14->anim.frames.push_back({ 397,9,80,34 });
	drone14->anim.frames.push_back({ 481,9,80,34 });
	drone14->anim.frames.push_back({ 565,9,80,34 });
	drone14->anim.speed = 3.0f;
	drone14->speed = { 120.0f,75.0f,7.5f };
	drone14->efxIndex = droneStartSFX;
	enemies["drone14"] = drone14;

	Enemy* drone15 = new Drone(8, graphics);
	drone15->anim.frames.push_back({ 397,9,80,34 });
	drone15->anim.frames.push_back({ 481,9,80,34 });
	drone15->anim.frames.push_back({ 565,9,80,34 });
	drone15->anim.speed = 3.0f;
	drone15->speed = { 180.0f,25.0f,7.5f };
	drone15->efxIndex = droneStartSFX;
	enemies["drone15"] = drone15;

	Enemy* drone16 = new Drone(8, graphics);
	drone16->anim.frames.push_back({ 397,9,80,34 });
	drone16->anim.frames.push_back({ 481,9,80,34 });
	drone16->anim.frames.push_back({ 565,9,80,34 });
	drone16->anim.speed = 3.0f;
	drone16->speed = { -180.0f,25.0f,7.5f };
	drone16->efxIndex = droneStartSFX;
	enemies["drone16"] = drone16;

	//Fly needs x=150 or x=0 
	Enemy* fly = new Fly(0,graphics);
	fly->anim.frames.push_back({573,146,169,69});
	fly->speed = {50.0f, -140.0f, -5.0f};
	fly->efxIndex = flyStartSFX;
	enemies["fly1"] = fly;

	Enemy* fly2 = new Fly(0, graphics);
	fly2->anim.frames.push_back({ 573,146,169,69 });
	fly2->speed = { 0.0f, -140.0f, -5.0f };
	fly2->efxIndex = flyStartSFX;
	enemies["fly2"] = fly2;

	Enemy* fly3 = new Fly(0, graphics);
	fly3->anim.frames.push_back({ 573,146,169,69 });
	fly3->speed = { -50.0f, -140.0f, -5.0f };
	fly3->efxIndex = flyStartSFX;
	enemies["fly3"] = fly3;

	Enemy* fly4 = new Fly(1, graphics);
	fly4->anim.frames.push_back({ 573,146,169,69 });
	fly4->speed = { 70.0f, -120.0f, -5.0f };
	fly4->efxIndex = flyStartSFX;
	enemies["fly4"] = fly4;

	Enemy* fly5 = new Fly(1, graphics);
	fly5->anim.frames.push_back({ 573,146,169,69 });
	fly5->speed = { -70.0f, -120.0f, -5.0f };
	fly5->efxIndex = flyStartSFX;
	enemies["fly5"] = fly5;

	Enemy* dragon1 = new Dragon1();
	dragon1->speed = {80.0f,80.0f,-5.0f};
	dragon1->hits = 6;
	Enemy* head1 = new BodyPart(graphics,dragon1);
	head1->anim.frames.push_back({81,8,67,105});
	head1->anim.frames.push_back({2,7,71,108});
	head1->anim.frames.push_back({ 81,124,67,105 });
	head1->anim.frames.push_back({ 2,123,71,108 });
	head1->anim.frames.push_back({ 81,8,67,105 });
	head1->anim.frames.push_back({ 2,7,71,108 });
	head1->anim.frames.push_back({ 81,240,67,105 });
	head1->anim.frames.push_back({ 2,239,71,108 });
	head1->anim.frames.push_back({ 81,8,67,105 });
	head1->anim.frames.push_back({ 2,7,71,108 });
	head1->anim.frames.push_back({ 81,356,67,105 });
	head1->anim.frames.push_back({ 2,355,71,108 });
	head1->anim.speed = 0;
	Enemy* body1_1 = new BodyPart(graphics,dragon1);
	body1_1->anim.frames.push_back({163,26,114,73});
	body1_1->anim.frames.push_back({ 163,142,114,73 });
	body1_1->anim.frames.push_back({ 163,26,114,73 });
	body1_1->anim.frames.push_back({ 163,258,114,73 });
	body1_1->anim.frames.push_back({ 163,26,114,73 });
	body1_1->anim.frames.push_back({ 163,374,114,73 });
	body1_1->anim.speed = 0;
	body1_1->destructible = false;
	Enemy* body1_2 = new BodyPart(graphics, dragon1);
	body1_2->anim.frames.push_back({ 163,26,114,73 });
	body1_2->anim.frames.push_back({ 163,142,114,73 });
	body1_2->anim.frames.push_back({ 163,26,114,73 });
	body1_2->anim.frames.push_back({ 163,258,114,73 });
	body1_2->anim.frames.push_back({ 163,26,114,73 });
	body1_2->anim.frames.push_back({ 163,374,114,73 });
	body1_2->anim.speed = 0;
	body1_2->destructible = false;
	Enemy* body1_3 = new BodyPart(graphics, dragon1);
	body1_3->anim.frames.push_back({ 163,26,114,73 });
	body1_3->anim.frames.push_back({ 163,142,114,73 });
	body1_3->anim.frames.push_back({ 163,26,114,73 });
	body1_3->anim.frames.push_back({ 163,258,114,73 });
	body1_3->anim.frames.push_back({ 163,26,114,73 });
	body1_3->anim.frames.push_back({ 163,374,114,73 });
	body1_3->anim.speed = 0;
	body1_3->destructible = false;
	Enemy* body1_4 = new BodyPart(graphics, dragon1);
	body1_4->anim.frames.push_back({ 163,26,114,73 });
	body1_4->anim.frames.push_back({ 163,142,114,73 });
	body1_4->anim.frames.push_back({ 163,26,114,73 });
	body1_4->anim.frames.push_back({ 163,258,114,73 });
	body1_4->anim.frames.push_back({ 163,26,114,73 });
	body1_4->anim.frames.push_back({ 163,374,114,73 });
	body1_4->anim.speed = 0;
	body1_4->destructible = false;
	Enemy* body1_5 = new BodyPart(graphics, dragon1);
	body1_5->anim.frames.push_back({ 163,26,114,73 });
	body1_5->anim.frames.push_back({ 163,142,114,73 });
	body1_5->anim.frames.push_back({ 163,26,114,73 });
	body1_5->anim.frames.push_back({ 163,258,114,73 });
	body1_5->anim.frames.push_back({ 163,26,114,73 });
	body1_5->anim.frames.push_back({ 163,374,114,73 });
	body1_5->anim.speed = 0;
	body1_5->destructible = false;
	Enemy* body1_6 = new BodyPart(graphics, dragon1);
	body1_6->anim.frames.push_back({ 163,26,114,73 });
	body1_6->anim.frames.push_back({ 163,142,114,73 });
	body1_6->anim.frames.push_back({ 163,26,114,73 });
	body1_6->anim.frames.push_back({ 163,258,114,73 });
	body1_6->anim.frames.push_back({ 163,26,114,73 });
	body1_6->anim.frames.push_back({ 163,374,114,73 });
	body1_6->anim.speed = 0;
	body1_6->destructible = false;
	Enemy* body1_7 = new BodyPart(graphics, dragon1);
	body1_7->anim.frames.push_back({ 163,26,114,73 });
	body1_7->anim.frames.push_back({ 163,142,114,73 });
	body1_7->anim.frames.push_back({ 163,26,114,73 });
	body1_7->anim.frames.push_back({ 163,258,114,73 });
	body1_7->anim.frames.push_back({ 163,26,114,73 });
	body1_7->anim.frames.push_back({ 163,374,114,73 });
	body1_7->anim.speed = 0;
	body1_7->destructible = false;
	Enemy* tail1 = new BodyPart(graphics,dragon1);
	tail1->anim.frames.push_back({286,18,96,91});
	tail1->anim.frames.push_back({ 286,134,96,91 });
	tail1->anim.frames.push_back({ 286,18,96,91 });
	tail1->anim.frames.push_back({ 286,250,96,91 });
	tail1->anim.frames.push_back({ 286,18,96,91 });
	tail1->anim.frames.push_back({ 286,366,96,91 });
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
	Enemy* head3_1 = new BodyPart(graphics, dragon3_1);
	head3_1->anim.frames.push_back({ 3,469,91,115 });
	head3_1->anim.frames.push_back({ 3,591,91,115 });
	head3_1->anim.frames.push_back({ 3,713,91,115 });
	head3_1->anim.frames.push_back({ 3,591,91,115 });
	head3_1->anim.frames.push_back({ 220,588,91,115 });
	head3_1->anim.frames.push_back({ 3,591,91,115 });
	head3_1->anim.frames.push_back({ 224,714,91,115 });
	head3_1->anim.speed = 0;
	Enemy* tail3_1 = new BodyPart(graphics, dragon3_1);
	tail3_1->anim.frames.push_back({ 209,478,96,91 });
	tail3_1->destructible = false;
	Enemy* body3_1_1 = new BodyPart(graphics, dragon3_1);
	body3_1_1->anim.frames.push_back({104,474,93,67});
	body3_1_1->anim.frames.push_back({ 104,596,93,67 });
	body3_1_1->anim.frames.push_back({ 104,718,93,67 });
	body3_1_1->anim.frames.push_back({ 104,596,93,67 });
	body3_1_1->anim.frames.push_back({ 321,593,93,67 });
	body3_1_1->anim.frames.push_back({ 104,596,93,67 });
	body3_1_1->anim.frames.push_back({ 325,719,93,67 });
	body3_1_1->anim.speed = 0;
	body3_1_1->destructible = false;
	Enemy* body3_1_2 = new BodyPart(graphics, dragon3_1);
	body3_1_2->anim.frames.push_back({ 104,474,93,67 });
	body3_1_2->anim.frames.push_back({ 104,596,93,67 });
	body3_1_2->anim.frames.push_back({ 104,718,93,67 });
	body3_1_2->anim.frames.push_back({ 104,596,93,67 });
	body3_1_2->anim.frames.push_back({ 321,593,93,67 });
	body3_1_2->anim.frames.push_back({ 104,596,93,67 });
	body3_1_2->anim.frames.push_back({ 325,719,93,67 });
	body3_1_2->anim.speed = 0;
	body3_1_2->destructible = false;
	Enemy* body3_1_3 = new BodyPart(graphics, dragon3_1);
	body3_1_3->anim.frames.push_back({ 104,474,93,67 });
	body3_1_3->anim.frames.push_back({ 104,596,93,67 });
	body3_1_3->anim.frames.push_back({ 104,718,93,67 });
	body3_1_3->anim.frames.push_back({ 104,596,93,67 });
	body3_1_3->anim.frames.push_back({ 321,593,93,67 });
	body3_1_3->anim.frames.push_back({ 104,596,93,67 });
	body3_1_3->anim.frames.push_back({ 325,719,93,67 });
	body3_1_3->anim.speed = 0;
	body3_1_3->destructible = false;
	Enemy* body3_1_4 = new BodyPart(graphics, dragon3_1);
	body3_1_4->anim.frames.push_back({ 104,474,93,67 });
	body3_1_4->anim.frames.push_back({ 104,596,93,67 });
	body3_1_4->anim.frames.push_back({ 104,718,93,67 });
	body3_1_4->anim.frames.push_back({ 104,596,93,67 });
	body3_1_4->anim.frames.push_back({ 321,593,93,67 });
	body3_1_4->anim.frames.push_back({ 104,596,93,67 });
	body3_1_4->anim.frames.push_back({ 325,719,93,67 });
	body3_1_4->anim.speed = 0;
	body3_1_4->destructible = false;
	Enemy* body3_1_5 = new BodyPart(graphics, dragon3_1);
	body3_1_5->anim.frames.push_back({ 104,474,93,67 });
	body3_1_5->anim.frames.push_back({ 104,596,93,67 });
	body3_1_5->anim.frames.push_back({ 104,718,93,67 });
	body3_1_5->anim.frames.push_back({ 104,596,93,67 });
	body3_1_5->anim.frames.push_back({ 321,593,93,67 });
	body3_1_5->anim.frames.push_back({ 104,596,93,67 });
	body3_1_5->anim.frames.push_back({ 325,719,93,67 });
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
	Enemy* head3_2 = new BodyPart(graphics, dragon3_2);
	head3_2->anim.frames.push_back({ 3,469,91,115 });
	head3_2->anim.frames.push_back({ 3,591,91,115 });
	head3_2->anim.frames.push_back({ 3,713,91,115 });
	head3_2->anim.frames.push_back({ 3,591,91,115 });
	head3_2->anim.frames.push_back({ 220,588,91,115 });
	head3_2->anim.frames.push_back({ 3,591,91,115 });
	head3_2->anim.frames.push_back({ 224,714,91,115 });
	head3_2->anim.speed = 0;
	Enemy* tail3_2 = new BodyPart(graphics, dragon3_2);
	tail3_2->anim.frames.push_back({ 209,478,96,91 });
	tail3_2->destructible = false;
	Enemy* body3_2_1 = new BodyPart(graphics, dragon3_2);
	body3_2_1->anim.frames.push_back({ 104,474,93,67 });
	body3_2_1->anim.frames.push_back({ 104,596,93,67 });
	body3_2_1->anim.frames.push_back({ 104,718,93,67 });
	body3_2_1->anim.frames.push_back({ 104,596,93,67 });
	body3_2_1->anim.frames.push_back({ 321,593,93,67 });
	body3_2_1->anim.frames.push_back({ 104,596,93,67 });
	body3_2_1->anim.frames.push_back({ 325,719,93,67 });
	body3_2_1->anim.speed = 0;
	body3_2_1->destructible = false;
	Enemy* body3_2_2 = new BodyPart(graphics, dragon3_2);
	body3_2_2->anim.frames.push_back({ 104,474,93,67 });
	body3_2_2->anim.frames.push_back({ 104,596,93,67 });
	body3_2_2->anim.frames.push_back({ 104,718,93,67 });
	body3_2_2->anim.frames.push_back({ 104,596,93,67 });
	body3_2_2->anim.frames.push_back({ 321,593,93,67 });
	body3_2_2->anim.frames.push_back({ 104,596,93,67 });
	body3_2_2->anim.frames.push_back({ 325,719,93,67 });
	body3_2_2->anim.speed = 0;
	body3_2_2->destructible = false;
	Enemy* body3_2_3 = new BodyPart(graphics, dragon3_2);
	body3_2_3->anim.frames.push_back({ 104,474,93,67 });
	body3_2_3->anim.frames.push_back({ 104,596,93,67 });
	body3_2_3->anim.frames.push_back({ 104,718,93,67 });
	body3_2_3->anim.frames.push_back({ 104,596,93,67 });
	body3_2_3->anim.frames.push_back({ 321,593,93,67 });
	body3_2_3->anim.frames.push_back({ 104,596,93,67 });
	body3_2_3->anim.frames.push_back({ 325,719,93,67 });
	body3_2_3->anim.speed = 0;
	body3_2_3->destructible = false;
	Enemy* body3_2_4 = new BodyPart(graphics, dragon3_2);
	body3_2_4->anim.frames.push_back({ 104,474,93,67 });
	body3_2_4->anim.frames.push_back({ 104,596,93,67 });
	body3_2_4->anim.frames.push_back({ 104,718,93,67 });
	body3_2_4->anim.frames.push_back({ 104,596,93,67 });
	body3_2_4->anim.frames.push_back({ 321,593,93,67 });
	body3_2_4->anim.frames.push_back({ 104,596,93,67 });
	body3_2_4->anim.frames.push_back({ 325,719,93,67 });
	body3_2_4->anim.speed = 0;
	body3_2_4->destructible = false;
	Enemy* body3_2_5 = new BodyPart(graphics, dragon3_2);
	body3_2_5->anim.frames.push_back({ 104,474,93,67 });
	body3_2_5->anim.frames.push_back({ 104,596,93,67 });
	body3_2_5->anim.frames.push_back({ 104,718,93,67 });
	body3_2_5->anim.frames.push_back({ 104,596,93,67 });
	body3_2_5->anim.frames.push_back({ 321,593,93,67 });
	body3_2_5->anim.frames.push_back({ 104,596,93,67 });
	body3_2_5->anim.frames.push_back({ 325,719,93,67 });
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
	App->textures->Unload(graphics);
	ASSERT(graphics != nullptr,AT("Enemies failed loading textures"));

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