#ifndef INPUTMAN_H
#define INPUTMAN_H

#include <SDL.h>
#include "ruleman.h"

enum Input {

	INPUT_A,
	INPUT_B,
	INPUT_C,
	INPUT_D,
	INPUT_1,
	INPUT_2,
	INPUT_3,
	INPUT_4,

	INPUT_UP,
	INPUT_DOWN,
	INPUT_LEFT,
	INPUT_RIGHT
	
};

enum Axis {
	AXIS_UP = -2,
	AXIS_DOWN = -3,
	AXIS_LEFT = -4,
	AXIS_RIGHT = -5,
};



class InputMan
{
	static InputMan* ms_instance;

private:
	InputMan(const InputMan& rhs);
	InputMan& operator=(const InputMan& rhs);
	int** binds; //key to input array, indexed by [player][input]
	bool** pressed; //output array, indexed similarly
	void checkInput(int in,int id,bool press); //check the physical key and update the output array
public:
	static InputMan* Instance();
	static void Release();
	bool HandleInput(SDL_Event &ev);
	inline int** GetBinds() { return binds; } //returns a handle to modify the keybindings
	inline const bool* GetPressed(int id) { //returns the output array
		if(id>=0 && id<RuleMan::Instance()->PlayerCount() )
			return pressed[id];
		return NULL;
		}
private:
	InputMan();
	~InputMan();

};

#endif // INPUTMAN_H
