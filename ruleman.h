#ifndef RULEMAN_H
#define RULEMAN_H


#include <vector>
#include <assert.h>

#include "cbrain.h"

#include "player.h"

enum GameState {
	GAME_INIT,
	GAME_MENU,
	GAME_GO,
	GAME_PAUSE,
	GAME_OVER,
	
	GAME_COUNT
	
};


typedef std::vector<Player> playerlist;

class RuleMan
{
	friend class CUnit;
	static RuleMan* ms_instance;

private:

	RuleMan(const RuleMan& rhs);
	RuleMan& operator=(const RuleMan& rhs);
	
	playerlist players;
	GameState state;
	
protected:
	inline void UnitMade(int team) {
		if(team>=0 && team<players.size() ) players[team].unitcount+=1;
	}
	inline void UnitKilled(int team) {
		if(team>=0 && team<players.size() ) players[team].unitcount-=1;
	}
public:
	static RuleMan* Instance();
	static void InitPlayers(int teamnum); // initializes the player list
	static void Release();
	inline Player* GetPlayer(int team) {
		assert(team<players.size() );
		return &players[team]; 
	}
	inline int PlayerCount() { return players.size(); }
	inline GameState GetState() {return state; }
private:
	RuleMan();
	~RuleMan();

};

#endif // RULEMAN_H
