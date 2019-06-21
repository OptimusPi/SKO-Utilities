#ifndef __OPI_GUI_H_
#define __OPI_GUI_H_

#include <list>
#include "OPI_Panel.h"

class OPI_Panel;

class OPI_Gui
{
public:

	enum class CursorType
	{
		Normal,
		Move,
		Resize,
		Hourglass
	};

	OPI_Gui();
	void addPanel(OPI_Panel *panel);
	void setCursor(CursorType selectedCursor);
	void initCursors(std::string normal, std::string move, std::string resize, std::string hourglass);
	void handleMouseMove(int mouseX, int mouseY);
	void handleMousePressLeft(int mouseX, int mouseY);
	void handleMousePressRight(int mouseX, int mouseY);
	void handleMouseReleaseLeft(int mouseX, int mouseY);
	void handleMouseReleaseRight(int mouseX, int mouseY);
	std::list<OPI_Panel*> panels;
private:
	SDL_Cursor *cursorNormal;
	SDL_Cursor *cursorMove;
	SDL_Cursor *cursorResize;
	SDL_Cursor *cursorHourglass;
};

#endif