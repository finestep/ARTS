#include "orders.h"

#include "cstdio"


Order::Order(ordertype t): type(t) {
	targetx=0;
	targety=0;
	targetid=-1;
	unittype tobuild=UNIT_COUNT;
}


void printOrder(Order &o ) {
	if(o.type == ORDER_MOVE ) {
		printf("MOVE: %.2f,%.2f ",o.targetx,o.targety);
	}
	else if(o.type == ORDER_ATTACK ) {
		printf("ATTACK: %i ",o.targetid);
	}
}

void printQueue(orderqueue &q) {
	int n = 0;
	printf("	");
	for(orderqueue::iterator iter = q.begin(); iter != q.end();iter++) {
		printf("%i: ",n);
		printOrder(*iter);
		n++;
	}
	printf("\n");
}