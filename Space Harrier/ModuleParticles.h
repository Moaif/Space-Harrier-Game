#ifndef __MODULEPARTICLES_H__
#define __MODULEPARTICLES_H__

#include<list>
#include "Globals.h"
#include "Module.h"
#include "Animation.h"
#include "Point.h"

struct SDL_Texture;
struct Collider;

struct Particle
{
	bool to_delete = false;
	bool firstSound=true;
	fPoint position = { 0, 0 , 1};
	unsigned int efxIndex;
	Animation anim;
	bool onlyOnce = false;
	float speed;

	SDL_Texture* texture;
	Collider* collider;
	SDL_Rect screenPoint = {0,0,0,0};


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

	SDL_Texture* lasers = nullptr;
	std::list<Particle*> active;

public:

	// prototype particles go here ...
	Particle laser;
	Particle explosion;
};

#endif // __MODULEPARTICLES_H__