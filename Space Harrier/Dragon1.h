#ifndef _DRAGON1_
#define _DRAGON1_

#include "Enemy.h"

class Dragon1 :public Enemy 
{
public:
	Dragon1(SDL_Texture* texture, Enemy* father = nullptr);
	~Dragon1();

	Enemy* Copy(const float& x, const float& y, const float& z, Enemy* father = nullptr)const override;
	void Update() override;
	void OnCollision(Collider* other) override;

private:
	Dragon1(const float& startingY,fPoint speed, SDL_Texture* texture, Enemy* father = nullptr);
	void Shoot();
	void Movement();

private:
	float elapsedTime;
	vector<fPoint> partSpeed;
	float startingY;

	static const int classPoints = 250000;
};

#endif // !_DRAGON1_

