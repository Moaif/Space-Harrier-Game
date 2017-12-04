#ifndef _FLY_
#define _FLY_

#include "Enemy.h"

class Fly :public Enemy {
public:
	Fly(int flyMode, SDL_Texture* texture, Enemy* father = nullptr);
	Fly(int flyMode, fPoint speed, SDL_Texture* texture, Enemy* father = nullptr);
	~Fly();

	Enemy* Copy(const float& x, const float& y, const float& z, Enemy* father = nullptr) const override;
	void Update() override;

private:
	void Shoot();
	void Mode0();
	void Mode1();

public:
	int flyMode;
private:
	static const float TIMEOFFSET;
	float elapsedTime;
	fPoint tempSpeed;
	bool shoted = false;
};

#endif // !_FLY_
