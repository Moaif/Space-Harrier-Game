#ifndef __MODULEPARTICLES_H__
#define __MODULEPARTICLES_H__

#include<list>
#include "Globals.h"
#include "Module.h"
#include "Animation.h"
#include "Point.h"
#include "ModuleCollision.h"

struct SDL_Texture;

struct Particle
{
	// TODO 1: Fill in the structure with all the properties you need for each particle
	bool to_delete = false;
	bool firstSound=true;
	fPoint position = { 0, 0 , 1};
	unsigned int efxIndex;
	Animation anim;
	bool onlyOnce = false;
	float speed;

	// TODO 11: Add an optional collider to each particle
	Collider* collider;


	Particle();
	Particle(const Particle& p);
	~Particle();

	void Update();
};

class ModuleParticles : public Module
{
public:
	ModuleParticles();
	~ModuleParticles();

	bool Start();
	update_status PreUpdate(); // clear dirty particles
	update_status Update(); // draw
	bool CleanUp();
	void OnCollision(Collider* col,Collider* other);

	void AddParticle(const Particle& particle, float x, float y); // feel free to expand this call

private:

	SDL_Texture* graphics = nullptr;
	std::list<Particle*> active;

public:

	// prototype particles go here ...
	Particle laser;
	Particle explosion;
};

#endif // __MODULEPARTICLES_H__