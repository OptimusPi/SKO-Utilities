#include "OPI_Image.h"
#include "ElementTheme.h"

#ifndef __OPI_GUI_ELEMENTTHEMEIMAGE_
#define __OPI_GUI_ELEMENTTHEMEIMAGE_

namespace OPI_Gui
{
	class OPI_ElementThemeImage : public ElementTheme
	{
	public:
		OPI_ElementThemeImage();
		virtual ~OPI_ElementThemeImage();
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