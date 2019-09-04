#ifndef __OPI_GUI_BUTTONTHEME_H_
#define __OPI_GUI_BUTTONTHEME_H_

#include "OPI_Image.h"
#include "OPI_GuiButton.h"

namespace OPI_Gui
{
	class Button;

	class ButtonTheme
	{
	public:
		ButtonTheme();
		virtual ~ButtonTheme();
		int getMinimumWidth();
		int getMinimumHeight();
		int getMaximumWidth();
		int getMaximumHeight();
		void render(OPI_Gui::Button* button);
		OPI_Image *textureEnabled;
		OPI_Image *textureDisabled;
		OPI_Image *texturePressed;
		OPI_Image *textureSelected;
	};
}

#endif