
#include "FontManager.h"

OPI_Text::FontManager * OPI_Text::FontManager::instance;

// Return true for success
bool OPI_Text::FontManager::init(std::string defaultFontPath)
{
	// Initialize SDL_Ttf
	OPI_Text::Font::InitializeSDL_ttf();

	// Initialize this singleton
	OPI_Text::FontManager* instance = getInstance();

	// Initialize font cache with a default font
	instance->loadedFonts = std::map<std::string, OPI_Text::Font*>();
	instance->addFont(instance->DEFAULT, defaultFontPath);
	return true;
}
 
void OPI_Text::FontManager::addFont(std::string fontName, std::string fontPath)
{
	auto font = new OPI_Text::Font(fontPath);

	// Save loaded font into dictionary
	OPI_Text::FontManager::instance->loadedFonts.insert({ fontName, font });
}

OPI_Text::Font* OPI_Text::FontManager::getFont(std::string fontName)
{
	return instance->loadedFonts[fontName];
}

OPI_Text::Font* OPI_Text::FontManager::getDefaultFont()
{
	return instance->loadedFonts[instance->DEFAULT];
}

OPI_Text::FontManager * OPI_Text::FontManager::getInstance()
{
	if (!OPI_Text::FontManager::instance)
	{
		OPI_Text::FontManager::instance = new OPI_Text::FontManager;
	}

	return OPI_Text::FontManager::instance;
}