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
		virtual bool handleMouseMove(int mouseX, int mouseY) = 0;
		virtual bool handleMousePressLeft(int mouseX, int mouseY) = 0;
		virtual bool handleMousePressRight(int mouseX, int mouseY) = 0;
		virtual bool handleMouseReleaseRight(int mouseX, int mouseY) = 0;
		virtual bool handleMouseReleaseLeft(int mouseX, int mouseY) = 0;
		virtual bool movableContainsMouse(int mouseX, int mouseY) = 0;
		virtual bool resizableContainsMouse(int mouseX, int mouseY) = 0;
		virtual bool closableContainsMouse(int mouseX, int mouseY) = 0;

		// Safe way to set the texture
		// Clear out existing texture and free SDL Surface
		void setTexture(OPI_Image *texture);
		OPI_Image *getTexture();
		int x;
		int y;
		int width;
		int height;
		bool isVisible;

		// Any GUI Element may have children, such as:
		// Panel can contain a few buttons and text
		// Buttons can contain 
		std::vector<OPI_Gui::Element *>children;
		OPI_Gui::Element *parent;
	private:
		OPI_Image *texture;
	};
}

#endif