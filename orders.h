#pragma once


#include <deque>
#include <map>
#include <utility>
#include <string>

#include "cunit.h"

enum ordertype {
	ORDER_MOVE,
	ORDER_ATTACK,
	ORDER_FOLLOW,
	ORDER_BUILD,
	ORDER_MINE,
	
	ORDER_NONE,
	
	ORDER_COUNT
	
};

struct Order {
	const ordertype type;
	float targetx,targety;
	int targetid;
	unittype tobuild;
	Order(ordertype type);
	//there will be more
};

typedef std::deque<Order> orderqueue;

typedef std::map<int,orderqueue > unitorders;

//helper function to create a move order to (x,y)
inline Order MoveOrder(float x, float y) {
	Order o(ORDER_MOVE);
	o.targetx = x;
	o.targety = y;
	return o;
}

//helper function to create an attack order against unit ID
inline Order AttackOrder(int id) {
	Order o(ORDER_ATTACK);
	o.targetid = id;
	return o;
}

void printOrder(Order &o);

void printQueue(orderqueue& q);
