#ifndef __OPI_GUI_ELEMENTRENDERER_
#define __OPI_GUI_ELEMENTRENDERER_

#include "Element.h"

namespace OPI_Gui
{
	class ElementRenderer
	{
	public:
		ElementRenderer();
		~ElementRenderer();
		virtual void render(OPI_Gui::Element *element) = 0;
	};
}

#endif
