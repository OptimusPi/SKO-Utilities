#include "OPI_Image.h"
#include "ElementTheme.h"

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
		void render(OPI_Gui::Element* element);
		OPI_Image* texture;
	private:
	};
}

#endif