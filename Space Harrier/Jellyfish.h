#ifndef _JELLYFISH_
#define _JELLYFISH_

#include "Enemy.h"

class Jellyfish :public Enemy 
{
public:
	Jellyfish(float bounceYMax,SDL_Texture* texture,Enemy* father=nullptr);
	~Jellyfish();

	Enemy* Copy() const override;
	void Update() override;

public:
	float bounceYMax;
};
#endif // !_JELLYFISH_

