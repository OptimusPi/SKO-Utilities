#ifndef	__OPI_RENDERER_H_
#define __OPI_RENDERER_H_

#include "OPI_Image.h"
#include "OPI_Text.h"
#include <string>


class OPI_Renderer
{
public:
	OPI_Renderer(std::string title, int windowWidth, int windowHeight);
	virtual ~OPI_Renderer();
	
	// Drawing functions
	void drawImage(int x, int y, const OPI_Image *img);
	void drawImage(int x, int y, const OPI_Image *img, float blendTolerance);
	void drawText(OPI_Text *text);
	void drawRect(SDL_Rect rect);

	// Screen functions
	void initScreen();
	void sizeScreen();
	void updateScreen();

	// Converting mouse from screen space to window render coordinates
	int getScaledMouseX(int mouse_x);
	int getScaledMouseY(int mouse_y);
	
	// SDL2 window
	SDL_Window *window = nullptr;

	// Window dimensions, helps with scaling
	int originalWindowWidth = 1920;
	int originalWindowHeight = 1080;
	// TODO ...make better scaling? :) 
	int windowWidth = 1280;
	int windowHeight = 720;
	// Window title
	std::string title = "";

	// OpenGL Context
	SDL_GLContext glContext = nullptr;

private:
	float defaultBlendTolerance = 0.75f;

};

#endif

