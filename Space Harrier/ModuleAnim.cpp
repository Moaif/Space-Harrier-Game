#include "Application.h"
#include "ModuleAnim.h"
#include "Animation.h"

using namespace std;

ModuleAnim::ModuleAnim() {

}

ModuleAnim::~ModuleAnim() {

}

bool ModuleAnim::CleanUp() {
	animations.clear();
	return true;
}

update_status ModuleAnim::Update() {
	for (list<Animation**>::iterator it = animations.begin(); it != animations.end(); ++it) {
		(*(*it))->Update();
	}
	return UPDATE_CONTINUE;
}

void ModuleAnim::SubscribeAnim(Animation** anim) {
	animations.push_back(anim);
}

void ModuleAnim::UnSubscribeAnim(Animation** anim) {
	int temp = static_cast<int>(animations.size());
	if (temp > 0) {
		for (list<Animation**>::iterator it = animations.begin(); it != animations.end(); ++it) {
			if ((*it) == anim) {
				animations.erase(it);
				return;
			}
		}
	}
}