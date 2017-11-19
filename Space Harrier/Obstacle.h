#ifndef _OBSTACLE_
#define _OBSTACLE_

#include "Enemy.h"

struct obstacleInfo;

class Obstacle : public Enemy {
public:
	Obstacle(bool destructible = true);
	~Obstacle();

	Enemy* Copy() const override;
	void Update() override;
private:
	const obstacleInfo* quad;
	float positionQuad;
	float lastFrameZ;
};

#endif // !_OBSTACLE_

