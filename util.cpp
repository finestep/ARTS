#include "util.h"


#include "cmap.h"

void getUnitsNear(int tilex, int tiley,CMap* map, std::set<int> &units, int width) {

	for(int ix=-width;ix<width;ix++)
		for(int iy=-width;iy<width;iy++) {
			std::set<int>* tileunits = map->getUnitsOnTile(tilex+ix,tiley+iy);
			if (tileunits!=NULL) units.insert( tileunits->begin(),tileunits->end() );
		}
}
