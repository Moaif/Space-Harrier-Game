#ifndef _MODULETIME_H_
#define _MODULETIME_H_

#include "Globals.h"
#include "Module.h"

class ModuleTime :public Module {
public:
	ModuleTime();
	~ModuleTime();

	update_status PreUpdate();

	float GetDeltaTime();
private:
	float previousTicks;
	float deltaTime;
};
#endif // !_MODULETIME_H_
