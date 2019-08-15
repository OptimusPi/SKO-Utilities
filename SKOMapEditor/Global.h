#ifndef __GLOBAL_H_
#define __GLOBAL_H_


#ifdef _WIN32
	#include "SDL.h" 
#else
	#include <SDL/SDL.h> 
#endif

extern SDL_Window *window; 

#endif