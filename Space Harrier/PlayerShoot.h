#ifndef _PLAYERSHOOT_
#define _PLAYERSHOOT_

#include "Particle.h"

class PlayerShoot : public Particle {
public:
	PlayerShoot(SDL_Texture* texture);
	~PlayerShoot();

	Particle* Copy(const float & x, const float & y, const float & z)const override;
	void Update() override;
	void OnCollision(Collider* other) override;

public:
	unsigned int reboundEfx;
private:
	float reboundXSpeed=0;

};

#endif // !_PLAYERSHOOT_

