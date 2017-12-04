#ifndef _DRONE_
#define _DRONE_

#include "Enemy.h"

class Drone :public Enemy {
public:
	Drone(int droneMode,SDL_Texture* texture,Enemy* father=nullptr);
	Drone(int droneMode,fPoint speed, SDL_Texture* texture, Enemy* father = nullptr);
	~Drone();

	Enemy* Copy(const float& x, const float& y, const float& z, Enemy* father = nullptr) const override;
	void Update() override;

private:
	void Shoot();
	void Mode0();
	void Mode1();
	void Mode2();
	void Mode3();

public:
	int droneMode;
private:
	static const float TIMEOFFSET;
	float elapsedTime;
	fPoint tempSpeed;
	bool shoted = false;
	bool bouncing = false;
};

#endif // !_DRONE_

