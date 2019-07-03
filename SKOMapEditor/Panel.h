#ifndef __Panel_H_
#define __Panel_H_

#include <vector>
#include <string>
#include "OPI_Image.h"
#include "Manager.h"
#include "Element.h"
#include "ElementThemeGridRect.h"
#include "ThemeLoader.h"

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
	class Manager;

	class Panel : public Element
	{
	public:
		Panel(std::string theme, int x, int y, int width, int height);
		virtual ~Panel();
		void setWidth(short int width);
		void setHeight(short int height);

		// Input handlers
		void handleMouseMove(int mouseX, int mouseY);
		void handleMousePressLeft(int mouseX, int mouseY);
		void handleMousePressRight(int mouseX, int mouseY);
		void handleMouseReleaseRight(int mouseX, int mouseY);
		void handleMouseReleaseLeft(int mouseX, int mouseY);
		bool movableContainsMouse(int mouseX, int mouseY);
		bool resizableContainsMouse(int mouseX, int mouseY);
		bool closableContainsMouse(int mouseX, int mouseY);

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
		bool isResizable = true;
		bool isMoving = false;
		int moveOriginX;
		int moveOriginY;
		int moveOriginGrabX;
		int moveOriginGrabY;
		bool isMovable = true;
		bool isClosable = true;
	};
}

#endif

