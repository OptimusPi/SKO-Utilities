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
#include "Font.h"

namespace OPI_Text
{
	class TextComponent
	{
	public:
		TextComponent();
		TextComponent(std::string content, OPI_Text::Font* font, int fontPoint = 12, bool wrapped = false);
		void setText(std::string content, OPI_Text::Font* font = nullptr, bool wrapped = false);
		void setSize(int fontPoint);
		size_t length();
		float R, G, B;
		float x, y; // TODO -- remove??? maybe

		unsigned short int width, height;
		bool visible;
		OPI_Image *contentRender;
		std::string content;

	private:
		OPI_Text::Font *font;
		int fontPoint;
		SDL_Color color;
		void renderImage(std::string content, OPI_Text::Font* font, bool wrapped);
	};

}

#endif
