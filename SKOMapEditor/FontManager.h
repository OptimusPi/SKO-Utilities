#ifndef __OPI_FONTS_H_
#define __OPI_FONTS_H_

// Standard libraries
#include <string>
#include <sstream>
#include <map>

// Platform dependent includes
#ifdef _WIN32
#include "SDL_ttf.h"
#elif defined __linux__
#include <SDL/SDL_ttf>
#elif defined __APPLE__
#include <SDL/SDL_ttf>
#endif

// Custom OPI Classes
#include "Font.h"
#include "TextComponent.h"

namespace OPI_Text
{
	class FontManager
	{
	public:
		static bool init(std::string defaultFontPath);
		static void addFont(std::string fontName, std::string fontPath);
		static OPI_Text::Font* getFont(std::string fontName);
		static OPI_Text::Font* getDefaultFont();
	private:
		static FontManager *instance;
		static FontManager * getInstance();
		FontManager() {};

		// Key used for the default font
		const std::string DEFAULT = "__DEFAULT__FONT__";

		// Cache for lazy loaded fonts
		std::map<std::string, OPI_Text::Font*> loadedFonts;
	};
}

#endif