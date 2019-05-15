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
	edges[0] = OPI_Image::getSurface(themePath + "edge-top-left.png");
	edges[1] = OPI_Image::getSurface(themePath + "edge-top-right.png");
	edges[2] = OPI_Image::getSurface(themePath + "edge-bottom-left.png");
	edges[3] = OPI_Image::getSurface(themePath + "edge-bottom-right.png");
	filler = OPI_Image::getSurface(themePath + "filler.png");
	canvas = OPI_Image::getSurface(themePath + "canvas.png");
}

void OPI_Panel::render(SDL_Surface *screen)
{
	// 
	// Form a blank canvas to draw on.
	//
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	Uint32 rmask = 0xff000000;
	Uint32 gmask = 0x00ff0000;
	Uint32 bmask = 0x0000ff00;
	Uint32 amask = 0x000000ff;
#else
	Uint32 rmask = 0x000000ff;
	Uint32 gmask = 0x0000ff00;
	Uint32 bmask = 0x00ff0000;
	Uint32 amask = 0xff000000;
#endif
/*
	SDL_Surface* panelCanvas = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 32,
		rmask, gmask, bmask, amask);*/

	//SDL_Surface *panelCanvas = filler;

	
	SDL_Surface* panelCanvas = SDL_CreateRGBSurface(SDL_HWSURFACE, this->width, this->height,
		24,
		0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);

	SDL_SetAlpha(panelCanvas, 0, 0);

	//
	// Draw pieces into the canvas.
	//
	// Start with corners
	SDL_Rect *topLeftCorner = new SDL_Rect { 0, 0, 8, 8 };
    int returnVal = SDL_BlitSurface(corners[0], topLeftCorner, panelCanvas, topLeftCorner);
	//if (returnVal)
	//	printf("Unable to render panel! SDL_BlitSurface failed.");
	SDL_Flip(panelCanvas);

	// Finally, convert slow SDL_Surface into OpenGL texture for fast drawing.
	texture = new OPI_Image(panelCanvas);
}