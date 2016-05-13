#include "updateman.h"

#include <cmath>
#include "cmap.h"

UpdateMan* UpdateMan::ms_instance = 0;

UpdateMan::UpdateMan()
{
	units = new unitlist();
	toadd = new unitlist();

}

UpdateMan::~UpdateMan()
{
	for(unitlist::iterator i=units->begin();i!=units->end();i++)
		delete *i;
	for(unitlist::iterator i=toadd->begin();i!=toadd->end();i++)
		delete *i;
	delete units;
	delete toadd;
}

void UpdateMan::UpdateUnits(float dt, CMap* map) {
	
	float drag = 6;

	unitlist::iterator j = units->begin();
	while(j!=units->end()) {
		CUnit* unit = *j;

		unit->force(-unit->getvelx()*drag*unit->stats.radius,-unit->getvely()*drag*unit->stats.radius);
		
		int tilex = unit->getposx()/map->tilesize;
		int tiley = unit->getposy()/map->tilesize;
		
		if(tilex != unit->tilex || tiley!=unit->tiley ) {
			map->remTileUnit(unit->id,unit->tilex,unit->tiley);
			map->addTileUnit(unit->id,tilex,tiley);
			unit->tilex=tilex;
			unit->tiley=tiley;
		}


		if((*j)->Update(dt,map) ) {
			map->remTileUnit(unit->id,unit->tilex,unit->tiley);
			delete *j;
			j = units->erase(j);
		} else {
			j++;
		}
	}
	
	for(unitlist::iterator i = toadd->begin();i!=toadd->end();i++ ) 
		units->push_back(*i);
	toadd->clear();
}
CUnit* UpdateMan::GetUnit(int id) {
	for(unitlist::iterator i = units->begin();i!=units->end();i++ ) 
		if((*i)->id == id) return *i;
	return NULL;
}
bool UpdateMan::IsAlive(int id) {
	for(unitlist::iterator i = units->begin();i!=units->end();i++ ) 
		if((*i)->id == id) return true;
	return false;
	
}


UpdateMan* UpdateMan::Instance()
{
	if (ms_instance == 0) {
		ms_instance = new UpdateMan();
	}
	return ms_instance;
}

void UpdateMan::Release()
{
	if (ms_instance) {
		delete ms_instance;
	}
	ms_instance = 0;
}

