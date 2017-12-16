#ifndef _ENEMYSHOOT_
#define _ENEMYSHOOT_

#include "Particle.h"

class EnemyShoot :public Particle {
public:
	EnemyShoot(SDL_Texture* texture);
	~EnemyShoot();

	Particle* Copy(const float & x, const float & y, const float & z)const override;
	void Update() override;
	void OnCollision(Collider* other) override;

private:
	fPoint initialPos;
	fPoint pathVector;
	bool timeScaled = false;
};

#endif // !_ENEMYSHOOT_

