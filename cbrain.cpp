#include "cbrain.h"

#include <cstdio>
#include <set>
#include <list>
#include <cmath>
#include "cmap.h"
#include "updateman.h"

CBrain::CBrain(int team): team(team),orders()
{
}

CBrain::~CBrain()
{
}

void CBrain::initID(int id) {
	if( orders.count(id)==0 ) {
		orders.insert(std::pair<int,orderqueue>(id,orderqueue() ) );
	}
}

void CBrain::GiveOrder(int id,Order order) {
	initID(id);
	orders[id].push_back( order );
	printOrders();
}

void inline CBrain::GiveOrderFirst(int id,Order order) {
	initID(id);
	orders[id].push_front( order );
}

void CBrain::ClearOrders(int id) {
	if( orders.count(id)>0 )
		orders[id].clear();
}

void CBrain::orderDone(int id) {
	initID(id);
	orders[id].pop_front();
}

Order* CBrain::getCurrentOrder(int id) {
	initID(id);
	if( orders[id].size()==0 ) return NULL;
	return &orders[id].front();
}

void CBrain::searchTargets(CUnit* unit,CMap* map, float rangeMul) {
	float range = unit->stats.weapon.range*rangeMul;
	int id = unit->id;
	UpdateMan* um = UpdateMan::Instance();

	if(unit == NULL) return;
	int tilex = unit->tilex;
	int tiley = unit->tiley;

	std::set<int> units;
	getUnitsNear(tilex,tiley,map,units,ceil(range/map->tilesize));
	
	float maxdist = range;
	CUnit* found = NULL;

	if(!units.empty() ) {
		CUnit* other;
		for(std::set<int>::iterator iter = units.begin();iter!=units.end();iter++) {
			other = um->GetUnit(*iter);
			if(other->team==unit->team) continue;
			float dx = unit->getposx() - other->getposx();
			float dy = unit->getposx() - other->getposx();
			float dist = sqrt( dx*dx + dy*dy );
			if(dist < maxdist) {
				found = other;
				maxdist = dist;
			}
		}
	}
			
	if(found!=NULL) {
		unit->target = found->id;
		if(maxdist>unit->stats.weapon.range)
			GiveOrder(id, AttackOrder(found->id) );
	}
}

void CBrain::stateAttack(CUnit* unit,CMap* map) {
		int id = unit->id;
		UpdateMan* um = UpdateMan::Instance();
		Order* o = getCurrentOrder(id);

		std::set<int> units;

		CUnit* other = um->GetUnit(o->targetid);
		if(unit==NULL) return;
		if(other==NULL) {
			orderDone(id);
			return;
		}
		
		unit->target = o->targetid;
		
		float dx = unit->getposx() - other->getposx();
		float dy = unit->getposy() - other->getposy();
		float dist = sqrt(dx*dx + dy*dy);

		if(dist>unit->stats.weapon.range) {
			
			float distToRange = std::min(unit->stats.weapon.range, (dist-unit->stats.weapon.range) );
			
			float movetargetx = unit->getposx()-dx/dist*distToRange;
			float movetargety = unit->getposy()-dy/dist*distToRange;
			GiveOrderFirst(id, MoveOrder(movetargetx,movetargety ) );
		}
}

void CBrain::stateMove(CUnit* unit,CMap* map) {

		 int id = unit->id;
		
		if(unit==NULL) return;
		
		Order* o = getCurrentOrder(id);

		float dx = unit->getposx()-o->targetx;
		float dy = unit->getposy()-o->targety;
		float dist = sqrt(dx*dx + dy*dy);
		
		if(dist<unit->stats.radius*3) {
			orderDone(id);
			unit->UpdateMovement(0,0);
			return;
		}
		
		float targetdir = 3.14+atan2(dy,dx);
		
		unit->UpdateMovement(dist / unit->stats.radius / 2, targetdir );

}

void CBrain::think(int id,CMap* map) {
	CUnit* unit = UpdateMan::Instance()->GetUnit(id);
	if(unit==NULL) return;
	if(unit->team!=team) {
		printf("Team %i unit %i's order on brain %i\n",unit->team,id,team);
		return;
	}
	if(orders[id].empty() ) {
		searchTargets(unit,map,2);
	} else {
		Order* order = getCurrentOrder(id);

		if(order->type == ORDER_ATTACK)
		{
			stateAttack( unit,map);
		}
		if(order->type == ORDER_MOVE)
		{
			stateMove( unit,map);
			searchTargets(unit,map);
		}
	}
	
}

void CBrain::Update(CMap* map) {
	std::list<int> todelete;
	unitorders::iterator iter = orders.begin();
	while(iter!=orders.end()) {
		if(UpdateMan::Instance()->IsAlive( iter->first ) ) {
			think(iter->first,map);
		} 
		else todelete.push_back(iter->first);
		iter++;
	}
	for(std::list<int>::iterator i = todelete.begin();i!=todelete.end();i++) orders.erase(*i);
	todelete.clear();
}

void CBrain::printOrders() {
	for(unitorders::iterator iter = orders.begin(); iter != orders.end();iter++) {
		printf("Unit %i:\n",iter->first );
		printQueue( iter->second );
	}

}