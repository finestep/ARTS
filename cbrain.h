#ifndef CBRAIN_H
#define CBRAIN_H

#include "cmap.h"
#include "orders.h"

class CMap;
struct Order;

enum unitstate {
	STATE_IDLE,
	STATE_MOVE,
	STATE_ATTACK,
	
	STATE_MOVETO,
	
	STATE_COUNT,
	
};

//Controls the units according to their orders

class CBrain
{
public:
	const int team;
	
	unitorders orders;

	void initID(int id);

	void stateMove(CUnit* unit,CMap* map); //moving an unit towards the next waypoint
	void stateAttack(CUnit* unit,CMap* map); //moving to range to attack or shooting
	void searchTargets(CUnit* unit,CMap* map,float rangeMul = 1); //search for valid targets

	void think(int id,CMap* map);

	void orderDone(int id); //removes current order from queue
	
	Order* getCurrentOrder(int id);

	
	CBrain(int team);
	~CBrain();
	
	void Update(CMap* map);

	void GiveOrder(int id,Order order); //issues an order on the back of the queue for the unit ID
	void GiveOrderFirst(int id,Order order); //issues an order, placing it on the front of the units queue
	void ClearOrders(int id);
	
	void printOrders();
};

#endif // CBRAIN_H
