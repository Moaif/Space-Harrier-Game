#ifndef _JELLYFISH_
#define _JELLYFISH_

#include "Enemy.h"

class Jellyfish :public Enemy 
{
public:
	Jellyfish(float bounceYMax,SDL_Texture* texture,Enemy* father=nullptr);
	~Jellyfish();

	Enemy* Copy(const float& x, const float& y, const float& z, Enemy* father = nullptr) const override;
	void Update() override;
	void OnCollision(Collider* other) override;

private:
	float bounceYMax;
	const obstacleInfo* quad;
	float positionQuad;
	float lastFrameZ;
	int counter = 0;

	static const int classPoints = 5000;
};
#endif // !_JELLYFISH_

