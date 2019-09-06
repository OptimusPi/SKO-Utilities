#ifndef __OPI_GUI_BUTTONTHEME_H_
#define __OPI_GUI_BUTTONTHEME_H_

#include "OPI_Image.h"
#include "Button.h"

namespace OPI_Gui
{
	class Button;

	class ButtonTheme
	{
	public:
		ButtonTheme();
		virtual ~ButtonTheme();
		virtual void render(OPI_Gui::Button *element) = 0;
		virtual int getMinimumWidth() = 0;
		virtual int getMinimumHeight() = 0;
		virtual int getMaximumWidth() = 0;
		virtual int getMaximumHeight() = 0;
		OPI_Image *textureEnabled;
		OPI_Image *textureDisabled;
		OPI_Image *texturePressed;
		OPI_Image *textureSelected;
	};
}

#endif