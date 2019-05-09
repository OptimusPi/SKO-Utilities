#ifndef __OPI_TEXT_H__
#define __OPI_TEXT_H__

// Standard libraries
#include <string>
#include <cstdlib>

// Platform dependent includes
#ifdef _WIN32
#include "SDL_ttf.h"
#elif defined __linux__
#include <SDL/SDL_ttf>
#elif defined __APPLE__
#include <SDL/SDL_ttf>
#endif


#include "OPI_Image.h"


class OPI_Text
{
      
	public:
		OPI_Text();
		OPI_Text(std::string content, TTF_Font* font = NULL, bool wrapped = false);
		static bool init();
		void setText(std::string content, TTF_Font* font = NULL, bool wrapped = false);
		size_t length();
		float R, G, B;
		float x, y;
		unsigned short int width, height;
		bool visible;
		OPI_Image contentRender;

	private:
		TTF_Font *currentFont;
		std::string content;
		SDL_Color color;
		void renderImage(std::string content, TTF_Font* font, bool wrapped);
};

#endif
