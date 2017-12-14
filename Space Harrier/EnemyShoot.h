#ifndef _ENEMYSHOOT_
#define _ENEMYSHOOT_

#include "Particle.h"

class EnemyShoot :public Particle {
public:
	EnemyShoot(SDL_Texture* texture);
	~EnemyShoot();

	Particle* Copy(const float & x, const float & y, const float & z)const override;
	void Update() override;

private:
	fPoint initialPos;
	fPoint pathVector;
};

#endif // !_ENEMYSHOOT_

