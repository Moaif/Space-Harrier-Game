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

	bool Start()override;
	update_status PreUpdate()override; 
	update_status Update()override; 
	bool CleanUp()override;

	void AddParticle(const Particle& particle, const float& x, const float& y,const float& z);
	void AddParticle(const Particle& particle, const float& x, const float& y,const float& z,const fPoint& unitaryVector);

private:

	SDL_Texture* lasers = nullptr;
	SDL_Texture* shots = nullptr;
	SDL_Texture* exp = nullptr;
	SDL_Texture* enemLaser = nullptr;
	list<Particle*> active;

public:
	list<Particle*> prototipeClearList;
	// prototype particles go here ...
	Particle* laser=nullptr;
	Particle* fire=nullptr;
	Particle* enemyLaser=nullptr;
	Particle* explosion=nullptr;
};

#endif // __MODULEPARTICLES_H__