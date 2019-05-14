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
	std::string themePath = "IMG/GUI/theme/" + theme + "/";

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
}
