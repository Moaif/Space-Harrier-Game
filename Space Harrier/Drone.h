#ifndef _DRONE_
#define _DRONE_

#include "Enemy.h"

class Drone :public Enemy {
public:
	Drone(SDL_Texture* texture,Enemy* father=nullptr);
	~Drone();

	Enemy* Copy(const float& x, const float& y, const float& z) const override;
	void Update() override;

private:
	const obstacleInfo* quad;
	int quadIndex;
	float positionQuad;
	float lastFrameZ;

	float elapsedTime;
};

#endif // !_DRONE_

