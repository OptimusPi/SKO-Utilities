#include <string>
#include <cstdlib>

#ifdef _WIN32
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_opengl.h"
#elif defined __APPLE__
#include <OpenGL/gl.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#elif defined __linux__
#include <GL/gl.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#endif 

#ifndef __OPI_IMAGE_H_
#define __OPI_IMAGE_H_

class OPI_Image
{
public:
	OPI_Image();
	~OPI_Image();
	OPI_Image(std::string filename);
	OPI_Image(OPI_Image *source);
	OPI_Image(SDL_Surface *surface);
	void setImage(std::string filename);
	void setImage(OPI_Image *source);
	void setImage(SDL_Surface *surface);
	static SDL_Surface* getSurface(std::string filePath);
	static GLuint generateTexture(SDL_Surface * surface);
	static SDL_Surface* createBlankSurface(unsigned short int width, unsigned short int height);
	static SDL_Surface* createColoredSurface(unsigned short width, unsigned short  height, unsigned char red, unsigned char green, unsigned char blue);
	GLuint texture;
	unsigned int width;
	unsigned int height;
};

#endif
