#ifndef __OPI_FONT_
#define __OPI_FONT_

// Platform dependent includes
#ifdef _WIN32
#include "SDL_ttf.h"
#elif defined __linux__
#include <SDL/SDL_ttf>
#elif defined __APPLE__
#include <SDL/SDL_ttf>
#endif

// Standard libraries
#include <string>
#include <sstream>
#include <map>

namespace OPI_Text
{
	class Font
	{
	public:
		// Constructors
		Font(std::string fontPath);
		Font();
		virtual ~Font();

		// Get the font at a given size
		TTF_Font * size(int fontPoint);

		static void InitializeSDL_ttf();

	private:
		// File path font is located
		std::string fontPath = "";

		// Load font at a certain size
		TTF_Font * loadFont(std::string fontPath, int fontPoint);

		// Cache all sizes and lazy load when needed
		std::map<int, TTF_Font*> fontSizes;
	};

}

#endif

