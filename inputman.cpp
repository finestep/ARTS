#include "inputman.h"

#include "ruleman.h"

InputMan* InputMan::ms_instance = 0;

InputMan::InputMan()
{
	binds = new int*[RuleMan::Instance()->PlayerCount()];
	pressed = new bool*[RuleMan::Instance()->PlayerCount()];
	for(int i=0;i<RuleMan::Instance()->PlayerCount();i++) {
		binds[i] = new int[12];
		pressed[i] = new bool[12];
		for(int j=0;j<12;j++) pressed[i][j] = false;
	}
}

InputMan::~InputMan()
{
	for(int i=0;i<RuleMan::Instance()->PlayerCount();i++) {
		delete binds[i];
		delete pressed[i];
	}
	delete binds;
	delete pressed;
}

InputMan* InputMan::Instance()
{
	if (ms_instance == 0) {
		ms_instance = new InputMan();
	}
	return ms_instance;
}

void InputMan::Release()
{
	if (ms_instance) {
		delete ms_instance;
	}
	ms_instance = 0;
}

void InputMan::checkInput(int in,int id,bool press) {
	for(int plr=0;plr<RuleMan::Instance()->PlayerCount();plr++) {
		for(int index=0;index<(id>=0?8:12);index++) {
			if(id!=RuleMan::Instance()->GetPlayer(plr)->devicenum) continue;
			if(binds[plr][index] == in) {
				pressed[plr][index]=press;
				return;
			}
		}
	}
}

bool InputMan::HandleInput(SDL_Event &ev) {
	if(ev.type == SDL_KEYDOWN || ev.type == SDL_KEYUP) {
		checkInput( ev.key.keysym.sym,-1,ev.key.state==SDL_PRESSED);
	} else if(ev.type == SDL_CONTROLLERBUTTONUP || ev.type == SDL_CONTROLLERBUTTONDOWN) {
		checkInput(ev.cbutton.button,ev.cbutton.which,ev.cbutton.state==SDL_PRESSED);
	} else if(ev.type == SDL_CONTROLLERAXISMOTION) {
		float x = ev.caxis.value/32767.0f;
		if(ev.caxis.axis==0) {
			checkInput(AXIS_UP,ev.caxis.which,x>0.5f);
			checkInput(AXIS_DOWN,ev.caxis.which,x<-0.5f);
		} else if(ev.caxis.axis==1) {
			checkInput(AXIS_LEFT,ev.caxis.which,x>0.5f);
			checkInput(AXIS_RIGHT,ev.caxis.which,x<-0.5f);
		}
	} else return false;
	return true;
}