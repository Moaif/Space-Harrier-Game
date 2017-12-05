#ifndef _BODYPART_
#define _BODYPART_

#include "Enemy.h"

class Particle;

class BodyPart :public Enemy {
public:
	BodyPart(SDL_Texture* texture, Enemy* father = nullptr);
	~BodyPart();

	Enemy* Copy(const float& x, const float& y, const float& z,  Enemy* father = nullptr)const override;
	void Update() override;
	void OnCollision(Collider* other)override;
	void Shoot(Particle* p);

};

#endif // !_BODYPART_
