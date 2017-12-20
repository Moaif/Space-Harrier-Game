#ifndef _MODULEANIM_H_
#define _MODULEANIM_H_

#include "Module.h"
#include "Globals.h"
#include <list>

class Animation;

class ModuleAnim : public Module {
public:
	ModuleAnim();
	~ModuleAnim();

	update_status Update()override;
	bool CleanUp() override;

	void SubscribeAnim(Animation** anim);
	void UnSubscribeAnim(Animation** anim);
private:
	std::list<Animation**>animations;
};

#endif // !_MODULEANIM_H_

