#ifndef __MODULEPARTICLES_H__
#define __MODULEPARTICLES_H__

#include<list>
#include "Globals.h"
#include "Module.h"
#include "Animation.h"
#include "Point.h"

class Particle;
struct SDL_Texture;
struct Collider;

class ModuleParticles : public Module
{
public:
	ModuleParticles();
	~ModuleParticles();

	bool Start();
	update_status PreUpdate(); 
	update_status Update(); 
	bool CleanUp();

	void AddParticle(const Particle* particle, float x, float y,float z);
	void AddParticle(const Particle* particle, float x, float y,float z,fPoint unitaryVector);

private:

	SDL_Texture* lasers = nullptr;
	SDL_Texture* shots = nullptr;
	std::list<Particle*> active;

public:

	// prototype particles go here ...
	Particle* laser;
	Particle* fire;
	Particle* explosion;
};

#endif // __MODULEPARTICLES_H__