#pragma once

#include <set>

#include "cmap.h"

class CMap;

inline void indexToCoord(int i,int& x, int& y,int w = 10,int h = 10) {
	x = i % w;
	y = i / h;
};

inline void coordToIndex(int x, int y, int& i, int w = 10, int h = 10) {
	i = x+y*w;
};

//helper function to get units at the tile (tilex,tiley) and radius surrounding tiles
void getUnitsNear(int tilex, int tiley,CMap* map, std::set<int> &units, int radius = 1);

