#ifndef __OPI_GUI_ELEMENTTHEMEBUTTON_
#define __OPI_GUI_ELEMENTTHEMEBUTTON_

#include "OPI_Image.h"
#include "OPI_GuiButtonTheme.h"
#include "OPI_GuiButton.h"

namespace OPI_Gui
{
	class Button;

	class ButtonThemeImageSet : public ElementTheme 
	{
	public:
		ButtonThemeImageSet();
		virtual ~ButtonThemeImageSet();
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