#include "ThemeLoader.h"
#include "ElementThemeGridRect.h"

/// Singleton instance
OPI_Gui::ThemeLoader * OPI_Gui::ThemeLoader::instance;
OPI_Gui::ThemeLoader * OPI_Gui::ThemeLoader::getInstance()
{
	if (!OPI_Gui::ThemeLoader::instance)
	{
		OPI_Gui::ThemeLoader::instance = new OPI_Gui::ThemeLoader;
	}

	return OPI_Gui::ThemeLoader::instance;
}

OPI_Gui::ElementTheme * OPI_Gui::ThemeLoader::GetTheme(std::string theme)
{
	// Return Element Theme either from memory cache
	// If not in memory cache, load from disk
	return 	getInstance()->getTheme(theme);
}

OPI_Gui::ElementTheme * OPI_Gui::ThemeLoader::getTheme(std::string theme)
{
	OPI_Gui::ElementTheme *elementTheme = nullptr;
	if (this->themes.count(theme) == 0)
	{
		this->loadTheme(theme);
	}

	return this->themes.at(theme);
}

// TODO - genericize this so it can be used for more than ElementThemeGridRect
void OPI_Gui::ThemeLoader::loadTheme(std::string theme)
{
	OPI_Gui::ElementThemeGridRect *elementThemeGridRect = new OPI_Gui::ElementThemeGridRect();

	std::string themePath = "IMG/GUI/themes/" + theme + "/";

	//Load template sprite, and then cut out the corners and middle
	SDL_Surface *panelTemplate = OPI_Image::getSurface(themePath + "panel.png");

	//Ensure it loaded
	if (panelTemplate == NULL)
	{
		throw "Panel Template is NULL. Maybe the image is missing from IMG/GUI/themes/" + theme + "/panel.png";
	}

	// Ensure it is a 3x3 "tileset" 
	if (panelTemplate->w % 3 > 0 || panelTemplate->h % 3 > 0)
	{
		throw "Panel template dimensions are incorrect! Width and Height must be divisible by 3.";
	}

	// Determine dimensions automatiacally from the template
	elementThemeGridRect->tileWidth = panelTemplate->w / 3;
	elementThemeGridRect->tileHeight = panelTemplate->h / 3;
	SDL_Rect tileClip{ 0, 0, elementThemeGridRect->tileWidth, elementThemeGridRect->tileHeight };

	// Top-Left corner
	elementThemeGridRect->corners[0] = OPI_Image::createBlankSurface(elementThemeGridRect->tileWidth, elementThemeGridRect->tileHeight);
	tileClip.x = 0;
	tileClip.y = 0;
	SDL_BlitSurface(panelTemplate, &tileClip, elementThemeGridRect->corners[0], NULL);

	// Top-Right corner
	elementThemeGridRect->corners[1] = OPI_Image::createBlankSurface(elementThemeGridRect->tileWidth, elementThemeGridRect->tileHeight);
	tileClip.x = elementThemeGridRect->tileWidth * 2;
	tileClip.y = 0;
	SDL_BlitSurface(panelTemplate, &tileClip, elementThemeGridRect->corners[1], NULL);

	// Bottom-Left corner
	elementThemeGridRect->corners[2] = OPI_Image::createBlankSurface(elementThemeGridRect->tileWidth, elementThemeGridRect->tileHeight);
	tileClip.x = 0;
	tileClip.y = elementThemeGridRect->tileHeight * 2;
	SDL_BlitSurface(panelTemplate, &tileClip, elementThemeGridRect->corners[2], NULL);

	// Bottom-Right corner
	elementThemeGridRect->corners[3] = OPI_Image::createBlankSurface(elementThemeGridRect->tileWidth, elementThemeGridRect->tileHeight);
	tileClip.x = elementThemeGridRect->tileWidth * 2;
	tileClip.y = elementThemeGridRect->tileHeight * 2;
	SDL_BlitSurface(panelTemplate, &tileClip, elementThemeGridRect->corners[3], NULL);

	// Top Edge
	elementThemeGridRect->edges[0] = OPI_Image::createBlankSurface(elementThemeGridRect->tileWidth, elementThemeGridRect->tileHeight);
	tileClip.x = elementThemeGridRect->tileWidth;
	tileClip.y = 0;
	SDL_BlitSurface(panelTemplate, &tileClip, elementThemeGridRect->edges[0], NULL);

	// Bottom Edge 
	elementThemeGridRect->edges[1] = OPI_Image::createBlankSurface(elementThemeGridRect->tileWidth, elementThemeGridRect->tileHeight);
	tileClip.x = elementThemeGridRect->tileWidth;
	tileClip.y = elementThemeGridRect->tileHeight * 2;
	SDL_BlitSurface(panelTemplate, &tileClip, elementThemeGridRect->edges[1], NULL);

	// Left Edge
	elementThemeGridRect->edges[2] = OPI_Image::createBlankSurface(elementThemeGridRect->tileWidth, elementThemeGridRect->tileHeight);
	tileClip.x = 0;
	tileClip.y = elementThemeGridRect->tileHeight;
	SDL_BlitSurface(panelTemplate, &tileClip, elementThemeGridRect->edges[2], NULL);

	// Right Edge
	elementThemeGridRect->edges[3] = OPI_Image::createBlankSurface(elementThemeGridRect->tileWidth, elementThemeGridRect->tileHeight);
	tileClip.x = elementThemeGridRect->tileWidth * 2;
	tileClip.y = elementThemeGridRect->tileHeight;
	SDL_BlitSurface(panelTemplate, &tileClip, elementThemeGridRect->edges[3], NULL);

	// Filler
	elementThemeGridRect->filler = OPI_Image::createBlankSurface(elementThemeGridRect->tileWidth, elementThemeGridRect->tileHeight);
	tileClip.x = elementThemeGridRect->tileWidth;
	tileClip.y = elementThemeGridRect->tileHeight;
	SDL_BlitSurface(panelTemplate, &tileClip, elementThemeGridRect->filler, NULL);

	SDL_FreeSurface(panelTemplate);

	this->themes.insert({ {theme, elementThemeGridRect} });
}

OPI_Gui::ThemeLoader::ThemeLoader()
{
}


OPI_Gui::ThemeLoader::~ThemeLoader()
{
}
