#ifndef __OPI_GUIELEMENTBUTTONTHEMEIMAGE_
#define __OPI_GUIELEMENTBUTTONTHEMEIMAGE_


#include "OPI_Image.h"
#include "OPI_GuiElementTheme.h"
#include "OPI_GuiButton.h"

namespace OPI_Gui
{
	class Button;

	class ElementThemeButtonImage : public ElementTheme
	{
	public:
		ElementThemeButtonImage();
		virtual ~ElementThemeButtonImage();
		int getMinimumWidth();
		int getMinimumHeight();
		int getMaximumWidth();
		int getMaximumHeight();
		void render(OPI_Gui::Element* element);
		void render(OPI_Gui::Button* button);
		OPI_Image *texture;
	};
}
#endif