#include "cunit.h"

#include <cmath>
#include <cstdlib>
#include <SDL.h>

#include "cmap.h"
#include "ruleman.h"
#include "updateman.h"

int CUnit::count = 0;
const float CUnit::maxvel = 350.0f;

CUnit::CUnit(unittype type,float x,float y, int team,unitstats stats): 
			posx(x), posy(y), velx(0.0f), vely(0.0f), dir(0.0f),spriteanimindex(0), spriteanimphase(0),
			reload(0.0f),turning(0.0f), movement(0.0f), targetx(0), targety(0), target(-1), tilex(0), tiley(0), team(team), health(stats.maxhealth),
			id(count++), type(type), stats(stats)
{

	RuleMan::Instance()->UnitMade(team);
}

CUnit::~CUnit()
{
	RuleMan::Instance()->UnitKilled(team);

}

#define pi 3.1415927
bool CUnit::Update(float dt, CMap* map) {


	dir+=stats.turnspeed*turning;
	
	float vel = sqrt(velx*velx+vely*vely);

	if(vel<stats.maxspeed) force(cos(dir)*stats.accel/10.f*movement,sin(dir)*stats.accel/10.f*movement);
	else {
		velx/=vel*stats.maxspeed;
		vely/=vel*stats.maxspeed;
	}
	move(dt);
	
	if(vel>maxvel) {
		velx*=maxvel/vel;
		vely*=maxvel/vel;
	}
	
	if(reload>0.0f) reload-=dt;
	
	if(target != -1 ) {
		CUnit* targetUnit = UpdateMan::Instance()->GetUnit(target);
		if(targetUnit!=NULL ) Attack( targetUnit );
			else target = -1;
	}
	

	while (dir<0.0f) dir+=2*pi;
	while (dir>2*pi) dir-=2*pi;
	if(health>stats.maxhealth) health=stats.maxhealth;
	return health<=0.0f;
}

void CUnit::Attack(CUnit* other) {
	if(reload>0.0f) return;
	targetx = other->getposx();
	targety = other->getposy();
	float dx = posx-targetx;
	float dy = posy-targety;
	float dist = sqrt(dx*dx+dy*dy);
	if(dist>stats.weapon.range) return;
	other->Damage(stats.weapon.damage);
	reload=stats.weapon.reloadtime;
}

#define PI 3.14

const void CUnit::Draw(SDL_Renderer* renderer,SDL_Texture* sprites) {
	if(stats.spriteindex<0) {
		SDL_SetRenderDrawColor(renderer,sin(team*PI/2)*255,cos(team*PI/2)*255,sin(team*PI*2/3)*255,255);
		SDL_RenderDrawPoint(renderer,posx,posy);
	}
	if(reload>stats.weapon.reloadtime*0.9) {
		SDL_SetRenderDrawColor(renderer,255,255,0,255);
		SDL_RenderDrawLine(renderer,posx,posy,targetx,targety);
	}
	
}

CUnit* CUnit::MakeUnit(unittype type,int team, float x, float y) {
	unitstats stats;
	switch (type) {
		default:
			stats.maxhealth = 30.0f;
			stats.accel = 300.0f;
			stats.radius = 0.5f;
			stats.turnspeed = 0.6;
			stats.maxspeed = 50.0f;

			stats.spriteindex = -1;
			stats.spriteanimlength = 0;
			stats.spriteanimtime = 0.0f;
			
			stats.weapon.reloadtime = 0.08f;
			stats.weapon.damage = 2.0f;
			stats.weapon.range = 12.0f;
			stats.weapon.effect = EFFECT_MG;
	}
	return new CUnit(type,x,y,team,stats);
}