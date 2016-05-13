#ifndef CUNIT_H
#define CUNIT_H

#include <math.h>

#include <SDL.h>

#include "cmap.h"

class CMap;


enum unittype {
	UNIT_SOLDIER,
	UNIT_BUILDER,
	UNIT_SPIDER,
	UNIT_CANNON,
	
	UNIT_COUNT
};

enum weaponeffect {
	EFFECT_MG,
	EFFECT_LASER,
	EFFECT_SPLASH,
	
	EFFECT_COUNT
};

struct weaponinfo {
	float reloadtime;
	float damage;
	float range;
	weaponeffect effect;
};

struct unitstats {
	
	float maxhealth;
	float radius;
	float accel;
	float turnspeed;
	float maxspeed;
	
	weaponinfo weapon;
	
	int spriteindex; //base sprite index
	int spriteanimlength; //how many frames in animation
	float spriteanimtime; //how many seconds per frame of animation

};

class CUnit
{
	friend class CBrain;
	static int count; //for assigning IDs
	float posx,posy;
	float velx,vely;
	float accx,accy;
	float dir;
	static const float maxvel; //absolute cap on unit velocities
	int spriteanimindex; //current anim frame
	float spriteanimphase; //time remaining on current frame
	float reload; //current reload progress

	inline void move(float dt) {
		posx+=velx*dt;
		posy+=vely*dt;
		velx+=accx*dt;
		vely+=accy*dt;
		accx=0.0f;
		accy=0.0f;
	}
	
	float turning, movement; //movement control

	
	void Attack(CUnit* other);
	
	int targetx,targety;
	
protected: 
	CUnit(unittype type,float x,float y, int team,unitstats stats); //called by MakeUnit
	
	int target; //ID of attack target

public:
	int tilex, tiley; //space partitioning, should be protected
	const int team;
	float health;
	const int id; //unique ID
	const unittype type;

	const unitstats stats;

	~CUnit();

	inline void force(float x, float y) {
		accx+=x/stats.radius;
		accy+=y/stats.radius;
	}
	inline float getposx() { return posx; }
	inline float getposy() { return posy; }

	inline float getvelx() { return velx; }
	inline float getvely() { return vely; }
	
	inline float getdir() { return dir; }
	
	inline void Damage(float d) { //todo add warning for negative damage
		if(d<0.0f) return;
		health -= d;
	}

	const void Draw(SDL_Renderer* renderer,SDL_Texture* sprites);
	bool Update(float dt, CMap* map); //return true if the unit has died
	
	inline void UpdateMovement(float move = 0.0f, float turn = 0.0f) { //todo warn about invalid movement requests
		if(move>=0.0f && move<10.0f) movement=move;
		else if(move>10.0f) movement=10.0f;
		else movement = 0.0;
		
		dir=turn;
	}
	
	static CUnit* MakeUnit(unittype type, int team,float x, float y);

};
#endif // CUNIT_H
