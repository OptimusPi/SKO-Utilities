#ifndef __OPI_FONTS_H_
#define __OPI_FONTS_H_

// Standard libraries
#include <string>
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
#include "OPI_Image.h"

class FontManager
{
public:
	FontManager();
	bool init();
	void addFont(std::string fontName, std::string fontPath);
	TTF_Font* getFont(std::string fontName);

private:
	std::map<std::string, TTF_Font*> loadedFonts;
};


#endif