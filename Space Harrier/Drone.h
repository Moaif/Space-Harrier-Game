#ifndef _DRONE_
#define _DRONE_

#include "Enemy.h"

class Drone :public Enemy {
public:
	Drone(int droneMode,SDL_Texture* texture,Enemy* father=nullptr);
	~Drone();

	Enemy* Copy(const float& x, const float& y, const float& z, Enemy* father = nullptr) const override;
	void Update() override;

private:
	Drone(int droneMode, fPoint speed, SDL_Texture* texture, Enemy* father = nullptr);
	void Shoot();
	void Mode3_1();
	void Mode3_2(bool doubleSoot);
	void Mode3_3();
	void Mode3_4();
	void Mode1_1();
	void Mode1_2();
	void Mode1_3();
	void Mode1_4();

public:
	int droneMode;
private:
	static const float TIMEOFFSET;
	float elapsedTime;
	fPoint tempSpeed;
	bool shoted = false;
	bool bouncing = false;

	static const int classPoints = 10000;
};

#endif // !_DRONE_

