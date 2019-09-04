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
		virtual void render(OPI_Gui::Button *element) = 0;
		virtual int getMinimumWidth() = 0;
		virtual int getMinimumHeight() = 0;
		virtual int getMaximumWidth() = 0;
		virtual int getMaximumHeight() = 0;
	};
}

#endif