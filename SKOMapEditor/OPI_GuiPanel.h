#ifndef __Panel_H_
#define __Panel_H_

#include <vector>
#include <string>
#include "OPI_Image.h"
#include "OPI_GuiManager.h"
#include "OPI_GuiElement.h"
#include "OPI_GuiElementThemeGridRect.h" // TODO is this needed
#include "OPI_GuiThemeLoader.h"

#ifdef _WIN32
	#include "SDL.h"
	#include "SDL_Image.h"
	#include "SDL_opengl.h" 
#else 
	#include <SDL/SDL.h>
	#include <SDL/SDL_image.h>
	#include <SDL/SDL_opengl.h> 
#endif

namespace OPI_Gui
{
	class GuiManager;

	class Panel : public Element
	{
	public:
		Panel(OPI_Gui::ElementThemeType type, std::string theme, int x, int y, int width = 0, int height = 0);
		virtual ~Panel();
		void setWidth(short int width);
		void setHeight(short int height);

		// Input handlers
		bool isInteracting();
	    bool handleMouseMove(int mouseX, int mouseY);
	    bool handleMousePressLeft(int mouseX, int mouseY);
	    bool handleMousePressRight(int mouseX, int mouseY);
	    bool handleMouseReleaseRight(int mouseX, int mouseY);
	    bool handleMouseReleaseLeft(int mouseX, int mouseY);
		bool movableContainsMouse(int mouseX, int mouseY);
		bool resizableContainsMouse(int mouseX, int mouseY);
		bool closableContainsMouse(int mouseX, int mouseY);

		// Actions available on this panel
		bool isResizable = false;
		bool isMovable = false;
		bool isClosable = false;

		// Set cursor
		void setCursor(CursorType cursor);

	private:
		OPI_Gui::ElementTheme *theme;
		SDL_Rect *moveableGrabArea;
		SDL_Rect *resizableGrabArea;
		SDL_Rect *closablePressArea;

		bool containsMouse(int mouseX, int mouseY, int x, int y, int w, int h);
		bool handleSection_Resize(int mouseX, int mouseY);
		bool handleSection_Move(int mouseX, int mouseY);
		bool handleSection_Close(int mouseX, int mouseY);
		bool isResizing = false;
		bool isMoving = false;
		bool isClosing = false;
		int moveOriginX;
		int moveOriginY;
		int moveOriginGrabX;
		int moveOriginGrabY;
	};
}

#endif

