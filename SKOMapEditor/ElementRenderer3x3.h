#pragma once
#include "ElementRenderer.h"
#ifndef __OPI_GUI_ELEMENTRENDERER3x3_
#define __OPI_GUI_ELEMENTRENDERER3x3_

namespace OPI_Gui
{
	class ElementRenderer3x3 : public ElementRenderer
	{
	public:
		ElementRenderer3x3(std::string theme);
		virtual ~ElementRenderer3x3();

	private:
		unsigned short int tileWidth;
		unsigned short int tileHeight;
		SDL_Surface *corners[4];
		SDL_Surface *edges[4];
		SDL_Surface *filler;
		std::string theme;
	};
}


#endif
