#include "SKO_ItemObject.h"

#ifndef	__SKO_TARGET_H_
#define __SKO_TARGET_H_

#define TARGET_BULLSEYE 0
#define TARGET_CRATE	1
#define TARGET_KEG	2

class SKO_Target
{
	public:
		int 	x = 0;
		int 	y = 0;
		int 	w = 0;
		int 	h = 0;
		int 	type = 0;
		bool 	active = true;
		int 	loot = 0;// TODO why not a vector of loot items, huh?
		unsigned long long int respawn_ticker = 0;
		int	required_weapon = 0;
		int picId = 0;

	SKO_Target();
	SKO_Target(int x_in, int y_in, int w_in, int h_in, int id_in, int loot_in);
	SKO_ItemObject getLootItem();
};

#endif
