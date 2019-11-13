#ifndef __SKO_PORTAL_H_
#define __SKO_PORTAL_H_


class SKO_Portal{

public:

	SKO_Portal();
	virtual ~SKO_Portal();

	//this portal rect
	int x = 0, y = 0, w = 0, h = 0;
	
	//level requirement
	int level_required = 0;

	//where does this portal throw you?
	int spawn_x = 0, spawn_y = 0;

	//what map does it throw you to?
	unsigned char mapId = 0;
};

#endif