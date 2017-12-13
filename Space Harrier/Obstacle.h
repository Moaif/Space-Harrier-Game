#ifndef _OBSTACLE_
#define _OBSTACLE_

#include "Enemy.h"

struct obstacleInfo;

class Obstacle : public Enemy {
public:
	Obstacle(SDL_Texture* texture, Enemy* father=nullptr);
	~Obstacle();

	Enemy* Copy(const float& x, const float& y, const float& z, Enemy* father = nullptr)const  override;
	void Update() override;
	void OnCollision(Collider* other)override;

private:
	const obstacleInfo* quad=nullptr;
	float positionQuad;
	float lastFrameZ;
	int counter = 0;

	static const int classPoints = 2500;
};

#endif // !_OBSTACLE_

