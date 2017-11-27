#ifndef _DRAGON3_
#define _DRAGON3_

#include "Enemy.h"

class Dragon3 :public Enemy {
public:
	Dragon3(SDL_Texture* texture, Enemy* father = nullptr);
	~Dragon3();

	Enemy* Copy(const float& x, const float& y, const float& z, Enemy* father = nullptr) const override;
	void Update() override;

private:
	void Shoot();
	void Movement();

private:
	float elapsedTime;
	bool shoted = false;
	bool bouncing = false;
};

#endif // !_DRAGON3_

