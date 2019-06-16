#ifndef __OPI_GUI_H_
#define __OPI_GUI_H_

#include "OPI_Panel.h"
#include <list>

class OPI_Gui
{
public:
	OPI_Gui();
	void addPanel(OPI_Panel *panel);

private:
	std::list<OPI_Panel> panels;
	SDL_Cursor *pointerNormal;
	SDL_Cursor *pointerMove;
	SDL_Cursor *pointerResize;
	SDL_Cursor *pointerHourglass;
};
#endif