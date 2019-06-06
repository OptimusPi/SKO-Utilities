#ifndef __OPI_PANEL_H_
#define __OPI_PANEL_H_

#include <string>
#include "OPI_Image.h"

#ifdef _WIN32
	#include "SDL.h"
	#include "SDL_Image.h"
	#include "SDL_opengl.h" 
#else 
	#include <SDL/SDL.h>
	#include <SDL/SDL_image.h>
	#include <SDL/SDL_opengl.h> 
#endif


class OPI_Panel
{
public:
	OPI_Panel(std::string theme, int x = 0, int y = 0, unsigned short int width = 3, unsigned short int height = 3);
	virtual ~OPI_Panel();
	void loadTheme(std::string theme);
	int x, y;
	void setWidth(short int width);
	void setHeight(short int height);

	OPI_Image *texture;
	void render();
private:
	unsigned short int tileWidth;
	unsigned short int tileHeight;
	SDL_Surface *corners[4];
	SDL_Surface *edges[4];
	SDL_Surface *filler;
	std::string theme;
	unsigned short int width, height;
};

#endif

