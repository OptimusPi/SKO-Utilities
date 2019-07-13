#include "OPI_Image.h"
#include "ElementTheme.h"
#include "Button.h";

#ifndef __OPI_GUI_ELEMENTTHEMEBUTTON_
#define __OPI_GUI_ELEMENTTHEMEBUTTON_

namespace OPI_Gui
{
	class Button;

	class ElementThemeButton : public ElementTheme 
	{
	public:
		ElementThemeButton();
		virtual ~ElementThemeButton();
		int getMinimumWidth();
		int getMinimumHeight();
		int getMaximumWidth();
		int getMaximumHeight();
		void render(OPI_Gui::Element* element);
		void render(OPI_Gui::Button* button);
		OPI_Image *textureEnabled;
		OPI_Image *textureDisabled;
		OPI_Image *texturePressed;
		OPI_Image *textureSelected;
	private:
		
	};
}

#endif