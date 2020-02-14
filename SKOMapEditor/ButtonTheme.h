#ifndef __OPI_GUI_BUTTONTHEME_H_
#define __OPI_GUI_BUTTONTHEME_H_

#include "OPI_Image.h"

namespace OPI_Gui
{
	class Button;

	class ButtonTheme
	{
	public:
		ButtonTheme();
		virtual ~ButtonTheme();
		void render(Button *button);
		int getMinimumWidth();
		int getMinimumHeight();
		int getMaximumWidth();
		int getMaximumHeight();
		OPI_Image *textureEnabled;
		OPI_Image *textureDisabled;
		OPI_Image *texturePressed;
		OPI_Image *textureSelected;
		std::string title = "";
	};
}

#endif