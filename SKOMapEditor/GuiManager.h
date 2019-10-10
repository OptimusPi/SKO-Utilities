#ifndef __OPI_GUI_H_
#define __OPI_GUI_H_

#include <list>

#include "SDL_headers.h"

#include "OPI_Renderer.h"
#include "CursorType.h"
#include "Element.h"
#include "Panel.h"

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
		static bool handleMouseMove(int mouseX, int mouseY);
		static bool handleMousePressLeft(int mouseX, int mouseY);
		static bool handleMousePressRight(int mouseX, int mouseY);
		static bool handleMouseReleaseLeft(int mouseX, int mouseY);
		static bool handleMouseReleaseRight(int mouseX, int mouseY); 

		// Get screen dimensions for help placing elements
		static int getScreenWidth();
		static int getScreenHeight();

		// Render an element
		void drawElement(int x, int y, OPI_Gui::Element *element);

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