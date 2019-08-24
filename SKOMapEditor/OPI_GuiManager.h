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
#include "OPI_Renderer.h"

namespace OPI_Gui
{
	/// Singleton
	class GuiManager
	{
	public:
		static void create(OPI_Renderer* renderer);
		static void addElement(OPI_Gui::Element *element);
		static void setCursor(OPI_Gui::CursorType selectedCursor);
		static void initCursors(std::string normal, std::string move, std::string resize, std::string hourglass, std::string hand);

		// Give priority for event handlers
		bool handleMouseMove_InteractingElements(int mouseX, int mouseY);

		// Mouse events
		static void handleMouseMove(int mouseX, int mouseY);
		static void handleMousePressLeft(int mouseX, int mouseY);
		static void handleMousePressRight(int mouseX, int mouseY);
		static void handleMouseReleaseLeft(int mouseX, int mouseY);
		static void handleMouseReleaseRight(int mouseX, int mouseY);

		// Get screen dimensions for help placing elements
		static int getScreenWidth();
		static int getScreenHeight();

		// All elements added to the GUI Manager
		std::vector<OPI_Gui::Element*> children;

		static GuiManager* getInstance();
	private:
		SDL_Cursor *cursorNormal;
		SDL_Cursor *cursorMove;
		SDL_Cursor *cursorResize;
		SDL_Cursor *cursorHourglass;
		SDL_Cursor *cursorHand;
		OPI_Renderer *renderer;
		int screenWidth;
		int screenHeight;


		static GuiManager * instance;
		GuiManager() {};
	};
}

#endif