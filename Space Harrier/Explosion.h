#ifndef _EXPLOSION_
#define _EXPLOSION_

#include "Particle.h"

class Explosion :public Particle {
public:
	Explosion(float fallSpeed,SDL_Texture* texture);
	~Explosion();

	Particle* Copy(const float & x, const float & y, const float & z)const override;
	void Update() override;

private:
	float fallSpeed;
};
#endif // !_EXPLOSION_

