#include "OPI_Image.h"

OPI_Image::OPI_Image()
{
	width = 0;
	height = 0;
	texture = 0;
}

OPI_Image::OPI_Image(std::string filename)
{
	setImage(filename);
}

OPI_Image::OPI_Image(OPI_Image *source)
{
	setImage(source);
}

OPI_Image::OPI_Image(SDL_Surface *surface)
{
	setImage(surface);
}

int pot(unsigned int x)
{
	return ((x != 0) && !(x & (x - 1)));
}

void OPI_Image::setImage(SDL_Surface * surface)
{
	//Clean up old texture memory from OpenGL
	if (texture)
	{
		glDeleteTextures(1, &texture);
	}

	width = surface->w;
	height = surface->h;

	if (!pot(width))
		printf("ERROR: OPI_Image width not power of two!\n");
	if (!pot(height))
		printf("ERROR: OPI_Image width not power of two!\n");

	texture = OPI_Image::generateTexture(surface);
}

void OPI_Image::setImage(std::string path)
{
	SDL_Surface *surface = OPI_Image::getSurface(path);
	setImage(surface);
	SDL_FreeSurface(surface);
}

void OPI_Image::setImage(OPI_Image *source)
{
	texture = source->texture;
	width = source->width;
	height = source->height;
}


SDL_Surface* OPI_Image::getSurface(std::string filePath)
{
	SDL_Surface *surface = IMG_Load(filePath.c_str());
	return surface;
}

GLuint OPI_Image::generateTexture(SDL_Surface * surface)
{
	GLuint tex[1];			// This is a handle to our texture object
	GLenum texture_format = GL_RGBA;
	GLint  nOfColors;

	if (surface)
	{
		// get the number of channels in the SDL surface
		nOfColors = surface->format->BytesPerPixel;
		if (nOfColors == 4)     // contains an alpha channel
		{

			if (surface->format->Rmask == 0x000000ff)
				texture_format = GL_RGBA;
			else
				texture_format = GL_BGRA;
		}
		else if (nOfColors == 3)     // no alpha channel
		{
			if (surface->format->Rmask == 0x000000ff)
				texture_format = GL_RGB;
			else
				texture_format = GL_BGR;
		}
		else {
			printf("WARNING: not truecolor OPI_Image! [%s]\n");
			texture_format = GL_RGBA;
		}


		// Have OpenGL generate a texture object handle for us
		glGenTextures(1, tex);

		// Bind the texture object
		glBindTexture(GL_TEXTURE_2D, tex[0]);


		// Set the texture's stretching properties
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Edit the texture object's OPI_Image data using the information SDL_Surface gives us
		glTexImage2D(GL_TEXTURE_2D, 0, nOfColors, surface->w, surface->h, 0,
			texture_format, GL_UNSIGNED_BYTE, surface->pixels);

		//TODO SDL_FreeSurface(surface);
	}

	return tex[0];
}