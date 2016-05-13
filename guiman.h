#ifndef GUIMAN_H
#define GUIMAN_H

#include "ruleman.h"
#include "cmap.h"
#include "orders.h"


#include "player.h"
#include "gui.h"

//very wip
class GUIMan
{
	static GUIMan* ms_instance;

private:
	GUIMan(const GUIMan& rhs);
	GUIMan& operator=(const GUIMan& rhs);
	void updateCursor(Player* plr);
	void countCursor(bool press,Player* plr,int& cursortime,int movex, int movey); //helper function for cursor movement

	void checkDrag(Player* plr,CMap* map); //cursor is dragging a box

	void addSelection(Player* plr, CMap* map); //add currently marked tiles to selection
	
	void checkOrders(Player* plr,CMap* map); //see if player has inputted orders

public:
	static GUIMan* Instance();
	static void Release();

	void Draw(SDL_Renderer* render, SDL_Texture* sprites);
	int cursorspeed;

private:
	GUIMan();
	~GUIMan();
	void Update(CMap* map);
};

#endif // GUIMAN_H
