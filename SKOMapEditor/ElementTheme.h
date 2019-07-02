#ifndef __OPI_GUI_ELEMENTTHEME_
#define __OPI_GUI_ELEMENTTHEME_

#include <string>

#include "Element.h"

namespace OPI_Gui
{
	class ElementTheme
	{
	public:
		ElementTheme() {};
		~ElementTheme() {};
		virtual void render(OPI_Gui::Element *element) = 0;
		virtual void loadtheme(std::string theme) = 0;
	private:

	};
}

#endif
