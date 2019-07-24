#pragma once
#include "OPI_GuiElementTheme.h"
#ifndef __OPI_GUI_ELEMENTTHEMEGRIDRECT_
#define __OPI_GUI_ELEMENTTHEMEGRIDRECT_

namespace OPI_Gui
{
	class ElementThemeGridRect : public ElementTheme
	{
	public:
		ElementThemeGridRect();
		virtual ~ElementThemeGridRect();
		int getMinimumWidth();
		int getMinimumHeight();
		int getMaximumWidth();
		int getMaximumHeight();
		void render(OPI_Gui::Element* element);
		unsigned short int tileWidth;
		unsigned short int tileHeight;
		SDL_Surface *corners[4];
		SDL_Surface *edges[4];
		SDL_Surface *filler;
	private:
		
	};
}


#endif
