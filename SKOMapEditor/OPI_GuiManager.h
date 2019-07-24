#ifndef __OPI_GUI_H_
#define __OPI_GUI_H_

#include <list>

#ifdef _WIN32
	#include "SDL.h"
#else 
	#include <SDL/SDL.h>
#endif

#include "OPI_GuiCursorType.h"
#include "OPI_GuiElement.h"
 
namespace OPI_Gui
{
	/// Singleton
	class GuiManager
	{
	public:
		static GuiManager* getInstance();
		void addElement(OPI_Gui::Element *element);
		void setCursor(OPI_Gui::CursorType selectedCursor);
		void initCursors(std::string normal, std::string move, std::string resize, std::string hourglass, std::string hand);

		// Give priority for event handlers
		bool handleMouseMove_InteractingElements(int mouseX, int mouseY);

		// Mouse events
		void handleMouseMove(int mouseX, int mouseY);
		void handleMousePressLeft(int mouseX, int mouseY);
		void handleMousePressRight(int mouseX, int mouseY);
		void handleMouseReleaseLeft(int mouseX, int mouseY);
		void handleMouseReleaseRight(int mouseX, int mouseY);

		// Recursive mouse events 
		std::vector<OPI_Gui::Element*> children;
	private:
		SDL_Cursor *cursorNormal;
		SDL_Cursor *cursorMove;
		SDL_Cursor *cursorResize;
		SDL_Cursor *cursorHourglass;
		SDL_Cursor *cursorHand;
		static GuiManager * instance;
		GuiManager() {};
	};
}

#endif