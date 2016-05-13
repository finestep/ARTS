#pragma once

inline void defaultKeyboard(int* inputs) {
	inputs[0]=SDLK_q;
	inputs[1]=SDLK_a;
	inputs[2]=SDLK_w;
	inputs[3]=SDLK_s;
	inputs[4]=SDLK_e;
	inputs[5]=SDLK_d;
	inputs[6]=SDLK_r;
	inputs[7]=SDLK_f;
	inputs[8]=SDLK_UP;
	inputs[9]=SDLK_DOWN;
	inputs[10]=SDLK_LEFT;
	inputs[11]=SDLK_RIGHT;
};

inline void defaultController(int* inputs) {
	inputs[0]=4;
	inputs[1]=3;
	inputs[2]=1;
	inputs[3]=5;
	inputs[4]=2;
	inputs[5]=6;
	inputs[6]=7;
	inputs[7]=8;
	inputs[8]=-2;
	inputs[9]=-3;
	inputs[10]=-4;
	inputs[11]=-5;
};