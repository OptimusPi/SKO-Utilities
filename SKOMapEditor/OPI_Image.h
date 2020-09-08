#include <string>
#include <cstdlib>

#include "SDL_headers.h"

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
	static SDL_Surface* copySurface(SDL_Surface *surface);
	static SDL_Surface* clipSurface(SDL_Surface* surface, unsigned int x, unsigned int y, unsigned int w, unsigned int h);
	static SDL_Surface* tintSurface(SDL_Surface *surface, unsigned int r, unsigned int g, unsigned int b, unsigned int a);
	static GLuint generateTexture(SDL_Surface * surface);
	static SDL_Surface* createBlankSurface(unsigned short int width, unsigned short int height);
	static SDL_Surface* createColoredSurface(unsigned short width, unsigned short  height, unsigned char red, unsigned char green, unsigned char blue);
	GLuint texture;
	unsigned int width;
	unsigned int height;
	std::string fileLocation = "";
	SDL_Surface* surface;
};

#endif
