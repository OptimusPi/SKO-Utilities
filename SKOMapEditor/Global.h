#ifndef __GLOBAL_H_
#define __GLOBAL_H_

#ifdef _WIN32
	#include "SDL.h"
#else
	#include <SDL/SDL.h>
#endif

// TODO - delete this, hope don't need

// press F to pay respects :'(

extern void saveMap();
extern bool fringe_mode;
extern char mode;

extern const char

TILE_DRAW, TILE_DELETE,
TOGGLE_FRINGE,
COLLISION_DRAW, COLLISION_DELETE,
TOGGLE_TEST,
EDIT_TILE, EDIT_COLLISION,
SAVE;


#endif
