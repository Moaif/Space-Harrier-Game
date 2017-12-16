#ifndef _MODULETIME_H_
#define _MODULETIME_H_

#include "Globals.h"
#include "Module.h"

class ModuleTime :public Module {
public:
	ModuleTime();
	~ModuleTime();

	update_status PreUpdate()override;

	void SetTimeScale(float value);

	float GetDeltaTime()const;
	float GetUnscaledDeltaTime() const;
	float GetTimeSinceStart()const;
private:
	float previousTicks;
	float deltaTime;
	float unscaledDeltaTime;
	float scale;
};
#endif // !_MODULETIME_H_
