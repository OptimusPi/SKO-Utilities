#ifndef __OPI_PANEL_H_
#define __OPI_PANEL_H_

#include <string>
#include "OPI_Image.h"
#include "OPI_Gui.h"
#include "OPI_Rectangle.h"

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
class Rectangle;

class OPI_Panel
{
public:
	OPI_Panel(OPI_Gui *gui, std::string theme, int x = 0, int y = 0, unsigned short int width = 3, unsigned short int height = 3);
	virtual ~OPI_Panel();
	void loadTheme(std::string theme);
	int x, y;
	bool isVisible = true;
	void setWidth(short int width);
	void setHeight(short int height);

	OPI_Image *texture;
	void render();

	// Input handlers
	void handleMouseMove(int mouseX, int mouseY);
	void handleMousePressLeft(int mouseX, int mouseY);
	void handleMousePressRight(int mouseX, int mouseY);
	void handleMouseReleaseRight(int mouseX, int mouseY);
	void handleMouseReleaseLeft(int mouseX, int mouseY);
	bool movableContainsMouse(int mouseX, int mouseY);
	bool resizableContainsMouse(int mouseX, int mouseY);
	bool closableContainsMouse(int mouseX, int mouseY);

private:
	
	OPI_Rectangle *moveableGrabArea;
	OPI_Rectangle *resizableGrabArea;
	OPI_Rectangle *closableGrabArea;

	unsigned short int tileWidth;
	unsigned short int tileHeight;
	SDL_Surface *corners[4];
	SDL_Surface *edges[4];
	SDL_Surface *filler;
	std::string theme;
	unsigned short int width, height;
	bool containsMouse(int mouseX, int mouseY, int x, int y, int w, int h);
	bool handleResize(int mouseX, int mouseY);
	bool handleMove(int mouseX, int mouseY);
	bool handleClose(int mouseX, int mouseY);
	bool isResizing = false;
	bool isResizable = true;
	bool isMoving = false;
	int moveOriginX;
	int moveOriginY;
	int moveOriginGrabX;
	int moveOriginGrabY;
	bool isMovable = true;
	bool isClosable = true;
	OPI_Gui *parent;
};

#endif

