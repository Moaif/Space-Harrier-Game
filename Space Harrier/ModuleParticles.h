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

	void AddParticle(const Particle* particle, const float& x, const float& y,const float& z);
	void AddParticle(const Particle* particle, const float& x, const float& y,const float& z,const fPoint& unitaryVector);

private:

	SDL_Texture* lasers = nullptr;
	SDL_Texture* shots = nullptr;
	SDL_Texture* exp = nullptr;
	list<Particle*> active;

public:
	list<Particle*> prototipeClearList;
	// prototype particles go here ...
	Particle* laser;
	Particle* fire;
	Particle* explosion;
};

#endif // __MODULEPARTICLES_H__