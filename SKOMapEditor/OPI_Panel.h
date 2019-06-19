#ifndef __OPI_PANEL_H_
#define __OPI_PANEL_H_

#include <string>
#include "OPI_Image.h"
#include "OPI_Gui.h"

#ifdef _WIN32
	#include "SDL.h"
	#include "SDL_Image.h"
	#include "SDL_opengl.h" 
#else 
	#include <SDL/SDL.h>
	#include <SDL/SDL_image.h>
	#include <SDL/SDL_opengl.h> 
#endif

class OPI_Gui;

class OPI_Panel
{
public:
	OPI_Panel(OPI_Gui *gui, std::string theme, int x = 0, int y = 0, unsigned short int width = 3, unsigned short int height = 3);
	virtual ~OPI_Panel();
	void loadTheme(std::string theme);
	int x, y;
	void setWidth(short int width);
	void setHeight(short int height);

	OPI_Image *texture;
	void render();

	// Input handlers
	void handleMouseMove(int mouseX, int mouseY);
	void handleMousePress(int mouseX, int mouseY);
	void handleMouseRelease(int mouseX, int mouseY);

private:
	unsigned short int tileWidth;
	unsigned short int tileHeight;
	SDL_Surface *corners[4];
	SDL_Surface *edges[4];
	SDL_Surface *filler;
	std::string theme;
	unsigned short int width, height;
	bool containsMouse(int mouseX, int mouseY, int x, int y, int w, int h);
	bool handleResize(int mouseX, int mouseY);
	bool isResizing = false;
	bool isResizable = true;
	OPI_Gui *parent;
};

#endif

