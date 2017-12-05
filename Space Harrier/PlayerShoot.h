#ifndef _PLAYERSHOOT_
#define _PLAYERSHOOT_

#include "Particle.h"

class PlayerShoot : public Particle {
public:
	PlayerShoot(SDL_Texture* texture);
	~PlayerShoot();

	Particle* Copy(const float & x, const float & y, const float & z)const override;
	void Update() override;

};

#endif // !_PLAYERSHOOT_

