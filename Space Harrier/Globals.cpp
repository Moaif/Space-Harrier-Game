#include "Globals.h"
#include <time.h>

bool seeded = false;

int getPseudoRand() {
	return rand();
}
int getRand() {

	if (!seeded) {
		srand((unsigned int)time(nullptr));
		seeded = true;
	}

	return rand();
}