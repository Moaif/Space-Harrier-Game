#ifndef _METALBALL_
#define _METALBALL_

#include "Enemy.h"
#include "Globals.h"

class MetalBall : public Enemy
{public:
	MetalBall(int ballMode, SDL_Texture* texture, Enemy* father = nullptr);
	~MetalBall();

	Enemy* Copy(const float& x, const float& y, const float& z, Enemy* father = nullptr) const override;
	void Update() override;
private:
	MetalBall(int ballMode, fPoint speed,float animSpeed, SDL_Texture* texture, Enemy* father = nullptr);
	void Shoot();
	void Mode1();

public:
	int ballMode;
private:
	static const float TIMEOFFSET;
	static const int MAX_ANIM_INDEX_INDESTRUCTIBLE;
	float elapsedTime;
	fPoint tempSpeed;
	float initialAnimSpeed;
	bool first = true;
	int numberOfShoots;

	static const int classPoints = 25000;
};

#endif // !_METALBALL_

