#ifndef __OPI_Gui_Element_
#define __OPI_Gui_Element_

#include "OPI_GuiCursorType.h"
#include "OPI_Image.h"
#include <vector>

namespace OPI_Gui
{
	class Element
	{
	public:
		// Input handlers
		virtual bool isInteracting();
		virtual bool handleMouseMove(int mouseX, int mouseY) = 0;
		virtual bool handleMousePressLeft(int mouseX, int mouseY) = 0;
		virtual bool handleMousePressRight(int mouseX, int mouseY) = 0;
		virtual bool handleMouseReleaseRight(int mouseX, int mouseY) = 0;
		virtual bool handleMouseReleaseLeft(int mouseX, int mouseY) = 0;

		// Safe way to set the texture
		// Clear out existing texture and free SDL Surface
		void setTexture(OPI_Image *texture);
		OPI_Image *getTexture();
		int x;
		int y;
		int width;
		int height;
		bool isVisible;

		// Calculates the bounds given by the parent element container
		int getMinimumX();
		int getMinimumY();
		int getMaximumX();
		int getMaximumY();
		void ensureBounds();

		// Add more elements to this element
		void addElement(OPI_Gui::Element *element);

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