#ifndef __OPI_GUIELEMENTBUTTONTHEMEIMAGE_
#define __OPI_GUIELEMENTBUTTONTHEMEIMAGE_


#include "OPI_Image.h"
#include "OPI_GuiButtonTheme.h"
#include "OPI_GuiButton.h"

namespace OPI_Gui
{
	class Button;

	class ButtonThemeIcon : public ButtonTheme
	{
	public:
		ButtonThemeIcon();
		virtual ~ButtonThemeIcon();
		int getMinimumWidth();
		int getMinimumHeight();
		int getMaximumWidth();
		int getMaximumHeight();
		void render(OPI_Gui::Element* element);
		void render(OPI_Gui::Button* button);

	};
}
#endif