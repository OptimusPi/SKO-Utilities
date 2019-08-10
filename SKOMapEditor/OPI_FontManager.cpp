
#include "OPI_FontManager.h"

OPI_FontManager * OPI_FontManager::instance;

// Return true for success
bool OPI_FontManager::init(std::string defaultFontPath)
{
	//Initialize SDL_ttf
	if (TTF_Init() == -1)
	{
		return false;
	}

	OPI_FontManager* instance = getInstance();
	instance->loadedFonts = std::map<std::string, TTF_Font*>();
	instance->addFont(instance->DEFAULT, defaultFontPath);

	return true;
}
 
void OPI_FontManager::addFont(std::string fontName, std::string fontPath)
{
	// Load font from file
	TTF_Font *font = TTF_OpenFont(fontPath.c_str(), 12);

	// If there was an error in loading the font
	if (font == NULL)
	{
		throw "Could not load font: [" + fontName + "]";
		return;
	}

	// Save loaded font into dictionary
	instance = OPI_FontManager::getInstance();
	instance->loadedFonts.insert({ fontName, font });
}

TTF_Font* OPI_FontManager::getFont(std::string fontName)
{
	return instance->loadedFonts[fontName];
}

TTF_Font* OPI_FontManager::getDefaultFont()
{
	return instance->loadedFonts[instance->DEFAULT];
}

OPI_FontManager * OPI_FontManager::getInstance()
{
	if (!OPI_FontManager::instance)
	{
		OPI_FontManager::instance = new OPI_FontManager;
	}

	return OPI_FontManager::instance;
}