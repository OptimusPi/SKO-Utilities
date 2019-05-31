#include "OPI_Panel.h"

OPI_Panel::OPI_Panel(std::string theme, int x, int y, unsigned short int width, unsigned short int height)
{
	this->theme = theme;
	this->x = x;
	this->y = y;
	this->width = width >= 16 ? width : 16;
	this->height = height >= 16 ? height : 16;
	this->loadTheme(theme);
}

OPI_Panel::~OPI_Panel() {}

void OPI_Panel::loadTheme(std::string theme)
{
	std::string themePath = "IMG/GUI/themes/" + theme + "/";

	// Load corners, edges, and filler from template sprites
	corners[0] = OPI_Image::getSurface(themePath + "corner-top-left.png");
	corners[1] = OPI_Image::getSurface(themePath + "corner-top-right.png");
	corners[2] = OPI_Image::getSurface(themePath + "corner-bottom-left.png");
	corners[3] = OPI_Image::getSurface(themePath + "corner-bottom-right.png");
	edges[0] = OPI_Image::getSurface(themePath + "edge-top.png");
	edges[1] = OPI_Image::getSurface(themePath + "edge-bottom.png");
	edges[2] = OPI_Image::getSurface(themePath + "edge-left.png");
	edges[3] = OPI_Image::getSurface(themePath + "edge-right.png");
	filler = OPI_Image::getSurface(themePath + "filler.png");
	canvas = OPI_Image::getSurface(themePath + "canvas.png");
}

void OPI_Panel::render()
{

	// https://wiki.libsdl.org/SDL_CreateRGBSurfaceFrom
	// Set up the pixel format color masks for RGBA byte arrays.
	// Note: when blitting images, the destination SDL_Rect ignores width and height.
		Uint32 rmask, gmask, bmask, amask;
	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		rmask = 0xff000000;
		gmask = 0x00ff0000;
		bmask = 0x0000ff00;
		amask = 0x000000ff;
	#else // little endian, like x86
		rmask = 0x000000ff;
		gmask = 0x0000ff00;
		bmask = 0x00ff0000;
		amask = 0xff000000;
	#endif

	// 
	// Form a blank canvas to draw on.
	// 
	SDL_Surface* panelCanvas = SDL_CreateRGBSurface(0, this->width, this->height,
		32,
		rmask, gmask, bmask, amask);

	// Fill panel background 
	for (short x = 8; x < this->width; x += 8)
	{
		for (short y = 8; y < this->height; y += 8)
		{
			//SDL_Rect fillArea = SDL_Rect{ x, y, 0, 0 };
			//SDL_BlitSurface(filler, NULL, panelCanvas, &fillArea);
		}
	}


	//
	// Draw border on top of the canvas
	//
	//
	// Draw Top and Bottom edges
	for (short x = 8; x < this->width - 8; x += 8)
	{
		SDL_Rect topEdge = SDL_Rect{ x, 0, 8, 8 };
		SDL_Rect bottomEdge = SDL_Rect{ x, this->height - 8, 8, 8 };
		SDL_BlitSurface(edges[0], NULL, panelCanvas, &topEdge);
		SDL_BlitSurface(edges[1], NULL, panelCanvas, &bottomEdge);
	}
	// Draw Left and Right edges
	for (short y = 8; y < this->height - 8; y += 8)
	{
		SDL_Rect leftEdge = SDL_Rect{ 0, y, 8, 8 };
		SDL_Rect rightEdge = SDL_Rect{ this->width - 8, y, 8, 8 };
		SDL_BlitSurface(edges[2], NULL, panelCanvas, &leftEdge);
		SDL_BlitSurface(edges[3], NULL, panelCanvas, &rightEdge);
	}
	// Cap off the edges with the corner pieces on top
	SDL_Rect topLeftCorner = SDL_Rect{ 0, 0, 8, 8 };
	SDL_Rect topRightCorner = SDL_Rect{ this->width - 8, 0, 0, 0 };
	SDL_Rect bottomLeftCorner = SDL_Rect{ 0, this->height - 8, 0, 0 };
	SDL_Rect bottomRightCorner = SDL_Rect{ this->width - 8, this->height - 8, 0, 0 };
	SDL_BlitSurface(corners[0], NULL, panelCanvas, &topLeftCorner);
	SDL_BlitSurface(corners[1], NULL, panelCanvas, &topRightCorner);
	SDL_BlitSurface(corners[2], NULL, panelCanvas, &bottomLeftCorner);
	SDL_BlitSurface(corners[3], NULL, panelCanvas, &bottomRightCorner);

	// Finally, convert slow SDL_Surface into OpenGL texture for fast drawing.
	delete texture;
	texture = new OPI_Image(panelCanvas);
	SDL_FreeSurface(panelCanvas);
}