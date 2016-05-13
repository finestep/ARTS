#pragma once

#include <vector>
#include <set>
#include <utility>

typedef std::pair<int,int> intpair;

enum cursordir {
	DIR_UP=0,
	DIR_DOWN,
	DIR_LEFT,
	DIR_RIGHT
};

enum guistate {
	GUI_IDLE,
	GUI_SELECTED,
	GUI_ORDER,
	GUI_DRAG
};
//contains the players GUI status
struct GUIInfo {
	struct cursor {
		int cursortime[4];
		int cursorx, cursory;
	} cursor;
	std::set< intpair > marked;
	std::vector<int> selection;
	guistate state;
	ordertype currentorder;
};
