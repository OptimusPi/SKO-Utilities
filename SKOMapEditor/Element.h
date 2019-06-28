#ifndef __OPI_Gui_Element_
#define __OPI_Gui_Element_

#include "CursorType.h"
#include "OPI_Image.h"
#include <vector>

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

		OPI_Image *texture;
		int x;
		int y;
		bool isVisible;

		// Any GUI Element may have children, such as:
		// Panel can contain a few buttons and text
		// Buttons can contain 
		std::vector<OPI_Gui::Element *>children;
		OPI_Gui::Element *parent;

	};
}

#endif