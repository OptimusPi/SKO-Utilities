
#include "OPI_FontManager.h"

OPI_FontManager * OPI_FontManager::instance;

// Return true for success
bool OPI_FontManager::init(std::string defaultFontPath)
{
	// Initialize SDL_Ttf
	OPI_Font::InitializeSDL_ttf();

	// Initialize this singleton
	OPI_FontManager* instance = getInstance();

	// Initialize font cache with a default font
	instance->loadedFonts = std::map<std::string, OPI_Font*>();
	instance->addFont(instance->DEFAULT, defaultFontPath);
	return true;
}
 
void OPI_FontManager::addFont(std::string fontName, std::string fontPath)
{
	auto font = new OPI_Font(fontPath);

	// Save loaded font into dictionary
	OPI_FontManager::instance->loadedFonts.insert({ fontName, font });
}

OPI_Font* OPI_FontManager::getFont(std::string fontName)
{
	return instance->loadedFonts[fontName];
}

OPI_Font* OPI_FontManager::getDefaultFont()
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