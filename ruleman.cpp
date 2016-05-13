#include "ruleman.h"

RuleMan* RuleMan::ms_instance = 0;

RuleMan::RuleMan(): players(), state(GAME_INIT)
{

}

RuleMan::~RuleMan()
{
	for(int i=0;i<players.size();i++) {
		delete players[i].brain;
	}
}

RuleMan* RuleMan::Instance()
{
	if (ms_instance == 0) {
		ms_instance = new RuleMan();
	}
	return ms_instance;
}

void RuleMan::Release()
{
	if (ms_instance) {
		delete ms_instance;
	}
	ms_instance = 0;
}

void RuleMan::InitPlayers(int teamnum) {
	RuleMan* rm = Instance();
	rm->players = playerlist();
	Player player;
	for(int i=0;i<teamnum;i++) {
		player.id = i;
		player.metal=0.0f;
		player.unitcount=0;
		player.brain = new CBrain(i);
		player.devicenum = -1;
		rm->players.push_back(player);
	}

}

