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
#include "OPI_Font.h"

class OPI_Text
{
	public:
		OPI_Text();
		OPI_Text(std::string content, OPI_Font* font, int fontPoint = 12, bool wrapped = false);
		void setText(std::string content, OPI_Font* font = nullptr , bool wrapped = false);
		void setSize(int fontPoint);
		size_t length();
		float R, G, B;
		float x, y; // TODO -- remove??? maybe

		unsigned short int width, height;
		bool visible;
		OPI_Image contentRender;

	private:
		OPI_Font *font;
		int fontPoint;
		std::string content;
		SDL_Color color;
		void renderImage(std::string content, OPI_Font* font, bool wrapped);
};

#endif
