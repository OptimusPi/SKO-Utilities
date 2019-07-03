#include "ElementThemeGridRect.h"

OPI_Gui::ElementThemeGridRect::ElementThemeGridRect()
{

}

OPI_Gui::ElementThemeGridRect::~ElementThemeGridRect()
{

}

int OPI_Gui::ElementThemeGridRect::getMinimumWidth()
{
	return this->tileWidth * 3;
}

int OPI_Gui::ElementThemeGridRect::getMinimumHeight()
{
	return this->tileHeight * 3;
}

void OPI_Gui::ElementThemeGridRect::render(OPI_Gui::Element* element)
{
	// 
	// Form a blank canvas to draw on.
	// 
	SDL_Surface *panelCanvas = OPI_Image::createBlankSurface(element->width, element->height);

	// Fill panel background 
	for (short x = this->tileWidth; x <= element->width - this->tileWidth; x += this->tileWidth)
	{
		bool edgeCase = false;
		unsigned short fillX = x;
		if (x >= element->width - this->tileWidth * 2)
		{
			edgeCase = true;
		}

		for (short y = this->tileHeight; y <=element->height - this->tileHeight; y += this->tileHeight)
		{
			unsigned short fillY = y;
			if (y >=element->height - this->tileHeight * 2)
			{
				edgeCase = true;
			}

			// Fill in middle from the top left corner without running over the corners or edges
			if (!edgeCase)
			{
				SDL_Rect fillArea = SDL_Rect{ x, y, this->tileWidth, this->tileHeight };
				SDL_BlitSurface(filler, NULL, panelCanvas, &fillArea);
			}


			// Fill in middle from the top left corner without running over the corners or edges
			if (edgeCase)
			{
				int fillHeight =element->height - this->tileHeight - fillY;
				int fillWidth = element->width - this->tileWidth - fillX;
				SDL_Rect clipArea = SDL_Rect{ 0, 0, fillWidth, fillHeight };
				SDL_Rect fillArea = SDL_Rect{ fillX, fillY, fillWidth, fillHeight };
				SDL_BlitSurface(filler, &clipArea, panelCanvas, &fillArea);
			}
		}
	}

	//
	// Draw border on top of the canvas
	//
	// Draw Top and Bottom edges
	for (short x = this->tileWidth; x <= element->width - this->tileWidth; x += this->tileWidth)
	{
		bool edgeCase = false;
		// Fill in gaps for when window width is not divisible by `tileSize`
		if (x > element->width - this->tileWidth * 2)
		{
			int fillWidth = element->width - this->tileWidth - x;
			SDL_Rect clipArea = SDL_Rect{ 0, 0, fillWidth, this->tileHeight };
			SDL_Rect topEdge = SDL_Rect{ x, 0, fillWidth, this->tileHeight };
			SDL_Rect bottomEdge = SDL_Rect{ x,element->height - this->tileHeight, this->tileWidth, this->tileHeight };
			SDL_BlitSurface(edges[0], &clipArea, panelCanvas, &topEdge);
			SDL_BlitSurface(edges[1], &clipArea, panelCanvas, &bottomEdge);
		}
		else
		{
			// Fill in edges from the start without running over the corners
			SDL_Rect topEdge = SDL_Rect{ x, 0, this->tileWidth, this->tileHeight };
			SDL_Rect bottomEdge = SDL_Rect{ x,element->height - this->tileHeight, this->tileWidth, this->tileHeight };

			SDL_BlitSurface(edges[0], NULL, panelCanvas, &topEdge);
			SDL_BlitSurface(edges[1], NULL, panelCanvas, &bottomEdge);
		}
	}

	// Draw Left and Right edges
	for (short y = this->tileHeight; y <=element->height - this->tileHeight; y += this->tileHeight)
	{
		bool edgeCase = false;

		// Fill in gaps for when window width is not divisible by `tileSize`
		if (y >element->height - this->tileHeight * 2)
		{
			int fillHeight =element->height - this->tileHeight - y;
			SDL_Rect clipArea = SDL_Rect{ 0, 0,
										this->tileWidth, fillHeight };
			SDL_Rect leftEdge = SDL_Rect{ 0, y,
										this->tileHeight, fillHeight };
			SDL_Rect rightEdge = SDL_Rect{ element->width - this->tileWidth, y,
										this->tileWidth, fillHeight };
			SDL_BlitSurface(edges[2], &clipArea, panelCanvas, &leftEdge);
			SDL_BlitSurface(edges[3], &clipArea, panelCanvas, &rightEdge);
		}
		else
		{
			// Fill in edges from the start without running over the corners
			SDL_Rect leftEdge = SDL_Rect{ 0, y, this->tileWidth, this->tileHeight };
			SDL_Rect rightEdge = SDL_Rect{ element->width - this->tileWidth, y, this->tileWidth, this->tileHeight };

			SDL_BlitSurface(edges[2], NULL, panelCanvas, &leftEdge);
			SDL_BlitSurface(edges[3], NULL, panelCanvas, &rightEdge);
		}
	}


	// Cap off the edges with the corner pieces on top
	SDL_Rect topLeftCorner = SDL_Rect{ 0, 0, this->tileWidth, this->tileHeight };
	SDL_Rect topRightCorner = SDL_Rect{ element->width - this->tileWidth, 0, 0, 0 };
	SDL_Rect bottomLeftCorner = SDL_Rect{ 0, element->height - this->tileHeight, 0, 0 };
	SDL_Rect bottomRightCorner = SDL_Rect{ element->width - this->tileWidth, element->height - this->tileHeight, 0, 0 };
	SDL_BlitSurface(corners[0], NULL, panelCanvas, &topLeftCorner);
	SDL_BlitSurface(corners[1], NULL, panelCanvas, &topRightCorner);
	SDL_BlitSurface(corners[2], NULL, panelCanvas, &bottomLeftCorner);
	SDL_BlitSurface(corners[3], NULL, panelCanvas, &bottomRightCorner);
	// Render with SDL2 and then convert to OpenGL texture
	delete element->texture;
	element->texture = new OPI_Image(panelCanvas);
	SDL_FreeSurface(panelCanvas);
}