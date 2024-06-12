#pragma once

#include "Common.h"

typedef struct Replay {
	char key;
	DWORD time;
	int type;

	Replay();
};

Replay::Replay() {
	type = 0;
}