#include "OPI_Image.h"
#include "OPI_GuiElementTheme.h"

#ifndef __OPI_GUI_ELEMENTTHEMEIMAGE_
#define __OPI_GUI_ELEMENTTHEMEIMAGE_

namespace OPI_Gui
{
	class ElementThemeImage : public ElementTheme
	{
	public:
		ElementThemeImage();
		virtual ~ElementThemeImage();
		int getMinimumWidth();
		int getMinimumHeight();
		int getMaximumWidth();
		int getMaximumHeight();
		void render(OPI_Gui::Element* element);
		OPI_Image* texture;
	private:
	};
}

#endif