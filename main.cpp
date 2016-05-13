#include <cmath>
#include <cstdio>
#include <iostream>
#include <string>

#define SDL_MAIN_HANDLED

#include <SDL.h>
#include <SDL_image.h>

#include "ruleman.h"
#include "updateman.h"
#include "cmap.h"
#include "cunit.h"

#include "orders.h"

#undef main

int main()
{
	if(SDL_Init(SDL_INIT_VIDEO) < 0) return -1;
	if(IMG_Init(IMG_INIT_PNG) < 0) return -2;
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
    SDL_CreateWindowAndRenderer(640,480,0,&window,&renderer);
	if(window==NULL) return -3;
	if(renderer==NULL) return -4;
	

	SDL_Texture* spritesheet = IMG_LoadTexture(renderer, "sprites.png");
	
	IMG_Quit();
	if(spritesheet==NULL) {
		
		 printf("IMG_Load: %s\n", IMG_GetError());
		 return -5;
		 
	}

	bool go = true;
	SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);
	SDL_RenderSetScale(renderer,3,3);
	
	RuleMan* rulemanager = RuleMan::Instance();
	
	rulemanager->InitPlayers(2);
	
	CMap map(32,32);
	CUnit* unit1 = CUnit::MakeUnit(UNIT_SOLDIER,0,3,3);
	UpdateMan::Instance()->AddUnit(unit1);
	CUnit* unit2 = CUnit::MakeUnit(UNIT_SOLDIER,1,63,63);
	UpdateMan::Instance()->AddUnit(unit2);

	rulemanager->GetPlayer(0)->brain->GiveOrder(unit1->id,AttackOrder(unit2->id)  );
	rulemanager->GetPlayer(1)->brain->GiveOrder(unit2->id, MoveOrder(50,3) );
	
	int mouseX, mouseY;
	float mousePosX,mousePosY;
    while(go)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
			
			int mouseB = SDL_GetMouseState(&mouseX,&mouseY);
			mousePosX=mouseX/3;
			mousePosY=mouseY/3;
            if (event.type == SDL_QUIT)
				go=false;
			if(event.type==SDL_KEYDOWN)
				if(event.key.keysym.sym==SDLK_ESCAPE) go=false;
			if(event.type==SDL_MOUSEBUTTONDOWN)
				if(mouseB&SDL_BUTTON(SDL_BUTTON_LEFT)) rulemanager->GetPlayer(0)->brain->GiveOrder(unit1->id, MoveOrder(mousePosX,mousePosY) );
				else rulemanager->GetPlayer(1)->brain->GiveOrder(unit2->id, MoveOrder(mousePosX,mousePosY) );
        }
		SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderFillRect(renderer,NULL);
		
		UpdateMan::Instance()->UpdateUnits(0.01f,&map);

		rulemanager->GetPlayer(0)->brain->Update(&map);
		rulemanager->GetPlayer(1)->brain->Update(&map);

		map.Draw(renderer,spritesheet);
		
		const unitlist* units = UpdateMan::Instance()->GetUnits();
		for(unitlist::const_iterator iter = units->begin(); iter!=units->end(); iter++ )
			(*iter)->Draw(renderer,spritesheet);

		SDL_RenderPresent(renderer);
		
		SDL_Delay(16);
    }
	UpdateMan::Release();
	RuleMan::Release();
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
    return 0;
}