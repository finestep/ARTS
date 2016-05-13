#include "guiman.h"

#include <set>
#include <vector>
#include <utility>

#include "inputman.h"
#include "updateman.h"
#include "ruleman.h"

GUIMan* GUIMan::ms_instance = 0;

GUIMan::GUIMan()
{	

	cursorspeed = 10;
		
}

GUIMan::~GUIMan()
{

}

GUIMan* GUIMan::Instance()
{
	if (ms_instance == 0) {
		ms_instance = new GUIMan();
	}
	return ms_instance;
}

void GUIMan::Release()
{
	if (ms_instance) {
		delete ms_instance;
	}
	ms_instance = 0;
}


void GUIMan::Update(CMap* map) {
	for(int i=0;i<RuleMan::Instance()->PlayerCount();i++) {
		Player* plr = RuleMan::Instance()->GetPlayer(i);
		updateCursor(plr);
		if(plr->gui.state==GUI_IDLE||plr->gui.state==GUI_DRAG||plr->gui.state==GUI_ORDER) {
			checkDrag(plr,map);
		} else if(plr->gui.state==GUI_SELECTED ) {
			checkOrders(plr,map);
		}
	}
}

void GUIMan::countCursor(bool pressed,Player* plr,int& cursortime,int movex, int movey) {

	if(pressed ) {
		cursortime++;
		if(cursortime>cursorspeed) {
			cursortime=0;
			plr->gui.cursor.cursorx+=movex;
			plr->gui.cursor.cursory+=movey;
		}
	} else {
		if(cursortime>cursorspeed/2 ) {
			plr->gui.cursor.cursorx+=movex;
			plr->gui.cursor.cursory+=movey;
		}
		cursortime=0;
	}

}

void GUIMan::updateCursor(Player* plr) {

	int id = plr->id;
	const bool* input = InputMan::Instance()->GetPressed(id);

	countCursor(input[INPUT_UP],plr,plr->gui.cursor.cursortime[DIR_UP],0,-1);

	countCursor(input[INPUT_DOWN],plr,plr->gui.cursor.cursortime[DIR_DOWN],0,1);
	
	countCursor(input[INPUT_LEFT],plr,plr->gui.cursor.cursortime[DIR_LEFT],-1,0);
	
	countCursor(input[INPUT_RIGHT],plr,plr->gui.cursor.cursortime[DIR_RIGHT],1,0);

}

void GUIMan::addSelection(Player* plr,CMap* map) {
	for(std::set< intpair >::iterator iter = plr->gui.marked.begin();iter != plr->gui.marked.end();iter++  ) {
		std::set<int>* units = map->getUnitsOnTile(iter->first,iter->second);

		for(std::set<int>::iterator unititer = units->begin(); unititer != units->end(); unititer++) {
			CUnit* unit = UpdateMan::Instance()->GetUnit(*unititer);
			if(unit!=NULL && unit->team == plr->id )
				plr->gui.selection.push_back( *unititer );
		}
	}
}

void GUIMan::checkDrag(Player* plr,CMap* map) {
	int id = plr->id;
	const bool* input = InputMan::Instance()->GetPressed(id);
	
	if(input[INPUT_A] ) {
		plr->gui.state=GUI_DRAG;
		plr->gui.marked.insert( intpair(plr->gui.cursor.cursorx,plr->gui.cursor.cursory ) );
	} else if(!input[INPUT_D] ) {
		if(plr->gui.currentorder < ORDER_NONE ) {
		//	issueOrder(plr,map);
		}
		else {
			addSelection(plr, map);
			plr->gui.state=GUI_SELECTED;
		}
	}	
	
}

void GUIMan::checkOrders(Player* plr,CMap* map) {
	int id = plr->id;
	const bool* input = InputMan::Instance()->GetPressed(id);
	const bool shift = input[INPUT_D];
	if(input[INPUT_A] ) {
		if(!shift) {
			plr->gui.selection.clear();
			return;
		} else {
			plr->gui.state = GUI_DRAG;
		}
	} else if( input[INPUT_B] ) {
		if(!shift) plr->brain->ClearOrders(plr->id);
		plr->gui.state=GUI_ORDER;
		plr->gui.currentorder=ORDER_MOVE;
	} else if( input[INPUT_C]) {
		if(!shift) plr->brain->ClearOrders(plr->id);
		plr->gui.state=GUI_ORDER;
		plr->gui.currentorder=ORDER_ATTACK;
	}
	
}

void GUIMan::Draw(SDL_Renderer* render, SDL_Texture* sprites) {
	SDL_Rect src,dst;
	const int size = 8;
	for(int id=0;id<RuleMan::Instance()->PlayerCount();id++) {
		src.w = size;
		src.h = size;
		dst.w = size;
		dst.h = size;
		src.x = 6;
		src.y = 28;
		dst.x=RuleMan::Instance()->GetPlayer(id)->gui.cursor.cursorx*dst.w;
		dst.y=RuleMan::Instance()->GetPlayer(id)->gui.cursor.cursory*dst.h;
		SDL_RenderCopy(render,sprites,&src,&dst);
	}
}
	