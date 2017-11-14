#ifndef _OBSTACLE_
#define _OBSTACLE_

#include "Enemy.h"

class Obstacle : public Enemy {
public:
	Obstacle(bool destructible = true, bool noDmg=false);
	~Obstacle();

	Enemy* Copy() const override;
	void Update() override;

private:
	bool destructible =true;
	bool noDmg = false;
};

#endif // !_OBSTACLE_

