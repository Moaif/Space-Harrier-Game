#ifndef _DRAGON1_
#define _DRAGON1_

#include "Enemy.h"

class Dragon1 :public Enemy 
{
public:
	Dragon1();
	~Dragon1();

	Enemy* Copy(const float& x, const float& y, const float& z, Enemy* father = nullptr)const override;
	void Update() override;
	void OnCollision(Collider* other) override;

private:
	Dragon1(fPoint speed);
	void Shoot();
	void Movement();
	void ReorderPartSpeed();

private:
	float elapsedTime;
	vector<fPoint> partSpeed;
	bool arrived = false;
	float shootTimer;

	static const float MAX_DRAGON_Z;
	static const float MOVEMENT_UPDATE_TIME;
	static const float SHOOT_INTERVAL;
	static const float MIN_Z_SHOOT;

	static const int classPoints = 250000;
};

#endif // !_DRAGON1_

