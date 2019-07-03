#pragma once
#include "ElementTheme.h"
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
