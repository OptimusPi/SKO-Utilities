
#include "OPI_FontManager.h"

FontManager::FontManager()
{
	this->loadedFonts = std::map<std::string, TTF_Font*>();
}

// Return true for success
bool FontManager::init()
{
	//Initialize SDL_ttf
	if (TTF_Init() == -1)
	{
		return false;
	}
	return true;
}
 
void FontManager::addFont(std::string fontName, std::string fontPath)
{
	// Load font from file
	TTF_Font *font = TTF_OpenFont(fontPath.c_str(), 12);

	// If there was an error in loading the font
	if (font == NULL)
	{
		printf("Could not load font: [%s]\n", fontName);
		return;
	}

	// Save loaded font into dictionary
	this->loadedFonts.insert({ fontName, font });
}

TTF_Font* FontManager::getFont(std::string fontName)
{
	return this->loadedFonts[fontName];
}