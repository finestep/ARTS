#ifndef CMAP_H
#define CMAP_H

#include <SDL.h>
#include <vector>
#include <set>

#include "util.h"
#include "cunit.h"

class CUnit;

typedef std::vector< unsigned char > tilemap;
typedef std::vector< std::set< int > > tileunits;

class CMap
{
	friend class UpdateMan;
	tilemap* tiles;
	tileunits* unittiles;
protected:
//for space partitioning, called from UpdateMan
	inline void addTileUnit(int id, int x, int y) {
		if(x < 0 || y < 0
			|| x>=sizex || y>=sizey) return;
		unittiles->at(x+y*sizex).insert(id);
	}
	inline void remTileUnit(int id, int x, int y){
		if(x < 0 || y < 0
			|| x>=sizex || y>=sizey) return;
		unittiles->at(x+y*sizex).erase(id);
	}
public:
	const int sizex,sizey;
	const int tilesize;
	CMap(int sizex,int sizey);
	~CMap();
	inline unsigned char GetTile(int x, int y) {

		return tiles->at(x+sizex*y);
	}
	inline void SetTile(int x, int y,unsigned char tile) {
		if(x >= 0 && y >= 0
			&& x<sizex && y<sizey)
		tiles->at(x+sizex*y) = tile;
	}
	void Draw(SDL_Renderer* render, SDL_Texture* sprites);
	
	std::set<int>* getUnitsOnTile(int x, int y) {
		if(x >= 0 && y >= 0
			&& x<sizex && y<sizey)
		return &unittiles->at(x+y*sizey);
		return NULL;
	}

};

#endif // CMAP_H
