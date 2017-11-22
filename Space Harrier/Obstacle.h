#ifndef _OBSTACLE_
#define _OBSTACLE_

#include "Enemy.h"

struct obstacleInfo;

class Obstacle : public Enemy {
public:
	Obstacle(SDL_Texture* texture, Enemy* father=nullptr);
	~Obstacle();

	Enemy* Copy(const float& x, const float& y, const float& z) const override;
	void Update() override;
private:
	const obstacleInfo* quad;
	float positionQuad;
	float lastFrameZ;
};

#endif // !_OBSTACLE_

