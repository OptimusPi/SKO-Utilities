#ifndef __GLOBAL_H_
#define __GLOBAL_H_

#include "SDL_headers.h"

// TODO - delete this, hope don't need

// press F to pay respects :'(

extern bool done;
extern char mode;
extern bool fringe_mode;
extern void saveMap();


// TODO enum
const char
TILE_DRAW = 1, TILE_DELETE = 2,
TOGGLE_FRINGE = 3,
COLLISION_DRAW = 4, COLLISION_DELETE = 5,
TOGGLE_TEST = 6,
EDIT_TILE = 7, EDIT_COLLISION = 8,
SAVE = 9;

#endif
