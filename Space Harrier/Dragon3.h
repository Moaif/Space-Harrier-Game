#ifndef _DRAGON3_
#define _DRAGON3_

#include "Enemy.h"

class Dragon3 :public Enemy {
public:
	Dragon3(int dragonMode,SDL_Texture* texture, Enemy* father = nullptr);
	Dragon3(int dragonMode,float startingY,fPoint speed,SDL_Texture* texture, Enemy* father = nullptr);
	~Dragon3();

	Enemy* Copy(const float& x, const float& y, const float& z, Enemy* father = nullptr) const override;
	void Update() override;

private:
	void Shoot();
	void Head1();
	void Head2();

public:
	int dragonMode=0;

private:
	static const float ARRIVALTIME;
	static const float TURNAMPLITUDE;
	static const float BOUNCEMAX;
	static const float TAILMOVEMENT;
	static const float TIMEOFFSET;
	float elapsedTime;
	fPoint headSpeed;
	fPoint tailSpeed;
	float startingY;
	bool shoted = false;
};

#endif // !_DRAGON3_

