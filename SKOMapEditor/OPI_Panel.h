#ifndef __OPI_PANEL_H_
#define __OPI_PANEL_H_

#include <string>
#include "OPI_Image.h"

class OPI_Panel
{
public:
	OPI_Panel(std::string theme, int x = 0, int y = 0, unsigned short int width = 32, unsigned short int height = 32);
	virtual ~OPI_Panel();
	void loadTheme(std::string theme);
	int x, y;
	OPI_Image *texture;
	void render();
private:
	SDL_Surface *corners[4];
	SDL_Surface *edges[4];
	SDL_Surface *filler;
	SDL_Surface *canvas;
	std::string theme;
	unsigned short int width, height;
};

#endif

