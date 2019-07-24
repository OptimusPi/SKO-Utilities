#ifndef __OPI_GUI_ELEMENTTHEME_
#define __OPI_GUI_ELEMENTTHEME_

#include <string>

#include "OPI_GuiElement.h"

namespace OPI_Gui
{
	class ElementTheme
	{
	public:
		ElementTheme() {};
		~ElementTheme() {};
		virtual void render(OPI_Gui::Element *element) = 0;
		virtual int getMinimumWidth() = 0;
		virtual int getMinimumHeight() = 0;
		virtual int getMaximumWidth() = 0;
		virtual int getMaximumHeight() = 0;
	private:

	};
}

#endif
