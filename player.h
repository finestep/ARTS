#pragma once

#include "gui.h"

struct Player {
	
	int id;
	
	CBrain * brain;
	
	int devicenum; //-1 for keyboard, 0>= for joysticks

	float metal;

	int unitcount;
	
	GUIInfo gui;

	
};
