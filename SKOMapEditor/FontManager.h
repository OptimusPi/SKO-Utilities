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
#include "OPI_Font.h"

class OPI_FontManager
{
public:
	static bool init(std::string defaultFontPath);
	static void addFont(std::string fontName, std::string fontPath);
	static OPI_Font* getFont(std::string fontName);
	static OPI_Font* getDefaultFont();
private:
	static OPI_FontManager *instance;
	static OPI_FontManager * getInstance();
	OPI_FontManager() {};

	// Key used for the default font
	const std::string DEFAULT = "__DEFAULT__FONT__";

	// Cache for lazy loaded fonts
	std::map<std::string, OPI_Font*> loadedFonts;
};


#endif