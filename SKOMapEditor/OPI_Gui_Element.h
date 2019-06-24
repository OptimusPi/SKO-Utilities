#ifndef __OPI_Gui_Element_
#define __OPI_Gui_Element_

#include "OPI_Gui_CursorType.h"
#include "OPI_Image.h"

namespace OPI_Gui
{
	class Element
	{
	public:
		// Input handlers
		virtual void handleMouseMove(int mouseX, int mouseY) = 0;
		virtual void handleMousePressLeft(int mouseX, int mouseY) = 0;
		virtual void handleMousePressRight(int mouseX, int mouseY) = 0;
		virtual void handleMouseReleaseRight(int mouseX, int mouseY) = 0;
		virtual void handleMouseReleaseLeft(int mouseX, int mouseY) = 0;
		virtual bool movableContainsMouse(int mouseX, int mouseY) = 0;
		virtual bool resizableContainsMouse(int mouseX, int mouseY) = 0;
		virtual bool closableContainsMouse(int mouseX, int mouseY) = 0;

		// Pass cursor event to parent
		void setCursor(CursorType cursor);

		OPI_Image *texture;
		int x;
		int y;
		bool isVisible;

	};
}

#endif