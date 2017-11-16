#ifndef _OBSTACLE_
#define _OBSTACLE_

#include "Enemy.h"

class Obstacle : public Enemy {
public:
	Obstacle(bool destructible = true);
	~Obstacle();

	Enemy* Copy() const override;
	void Update() override;
private:
	const SDL_Rect* quad;
	float positionQuad;
};

#endif // !_OBSTACLE_

