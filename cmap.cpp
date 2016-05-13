#include "cmap.h"

#include <SDL.h>
#include "util.h"
#include "math.h"

CMap::CMap(int sx,int sy): sizex(sx), sizey(sy), tilesize(6)
{
	tiles = new tilemap(sx*sy,0);
	unittiles = new tileunits(sx*sy,std::set< int >() );
}

CMap::~CMap()
{
	delete tiles;
}

void CMap::Draw(SDL_Renderer* render,SDL_Texture* sprites) {
	SDL_Rect src, dst;

	for(int x=0;x<sizex;x++)
		for(int y=0;y<sizey;y++) {
			src.w = tilesize;
			src.h = tilesize;
			dst.w = tilesize;
			dst.h = tilesize;
			int index = GetTile(x,y);
			if(index==0) index= (unsigned int)(sin(x*1556+y*843)-2*cos(x*2165+y*122) )%2;
			indexToCoord( index , src.x, src.y);
			src.x*=tilesize;
			src.y*=tilesize;
			dst.x=x*dst.w;
			dst.y=y*dst.h;
			SDL_RenderCopy(render,sprites,&src,&dst);
		}
}
