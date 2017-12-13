#ifndef _FLY_
#define _FLY_

#include "Enemy.h"

class Fly :public Enemy {
public:
	Fly(int flyMode, SDL_Texture* texture, Enemy* father = nullptr);
	~Fly();

	Enemy* Copy(const float& x, const float& y, const float& z, Enemy* father = nullptr) const override;
	void Update() override;

private:
	Fly(int flyMode, fPoint speed, SDL_Texture* texture, Enemy* father = nullptr);
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

	static const int classPoints = 20000;
};

#endif // !_FLY_
