#include <typeinfo>

#include "ElementThemeFactory.h"
#include "ElementThemeGridRect.h"
#include "ElementThemeImage.h"
#include "ButtonTheme.h"

/// Singleton instance
OPI_Gui::ElementThemeFactory * OPI_Gui::ElementThemeFactory::instance;
OPI_Gui::ElementThemeFactory * OPI_Gui::ElementThemeFactory::getInstance()
{
	if (!OPI_Gui::ElementThemeFactory::instance)
	{
		OPI_Gui::ElementThemeFactory::instance = new OPI_Gui::ElementThemeFactory;
	}

	return OPI_Gui::ElementThemeFactory::instance;
}

OPI_Gui::ElementTheme * OPI_Gui::ElementThemeFactory::GetTheme(OPI_Gui::ElementThemeType type, std::string theme)
{
	return 	getInstance()->getTheme(type, theme);
}

OPI_Gui::ElementTheme * OPI_Gui::ElementThemeFactory::getTheme(OPI_Gui::ElementThemeType type, std::string theme)
{
	// Create compisite key for the dictionary lookup in the theme cache
	// Example: "ElementThemeGridRect|ice" or "ElementThemeImage|stone"
	std::string key = generateKey(type, theme);
	OPI_Gui::ElementTheme *elementTheme = nullptr;

	// If not in memory cache, load from disk
	if (this->themes.count(key) == 0)
	{
		switch (type)
		{
		case ElementThemeType::Blank:
			break;
		case ElementThemeType::GridRect:
			this->loadTheme_GridRect(theme);
			break;
		case ElementThemeType::Image: 
			 this->loadTheme_Image(theme);
			break;
		default:
			throw std::logic_error("Invalid type provided to OPI_Gui::ElementThemeFactory!");
		}
	}

	// Return Element Theme from memory cache
	return this->themes.at(key);
}

void OPI_Gui::ElementThemeFactory::loadTheme_Image(std::string theme)
{
	OPI_Gui::OPI_ElementThemeImage *elementThemeImage = new OPI_Gui::OPI_ElementThemeImage();
	std::string themePath = "IMG/GUI/themes/panel_images/" + theme + ".png";
	elementThemeImage->texture = new OPI_Image(themePath);

	// Insert into cache
	std::string key = generateKey(OPI_Gui::ElementThemeType::Image, theme);
	this->themes.insert({ {key, elementThemeImage} });
}

void OPI_Gui::ElementThemeFactory::loadTheme_GridRect(std::string theme)
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

	//TODO simply use the inner width somehow
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

	std::string key = generateKey(OPI_Gui::ElementThemeType::GridRect, theme);
	this->themes.insert({ {key, elementThemeGridRect} });
}

std::string OPI_Gui::ElementThemeFactory::generateKey(OPI_Gui::ElementThemeType type, std::string theme)
{
	std::string key = "";

	switch (type)
	{
	case OPI_Gui::ElementThemeType::Blank:
		return "";
	case OPI_Gui::ElementThemeType::Image:
		key += "Image|";
		break;
	case OPI_Gui::ElementThemeType::GridRect:
		key += "GridRect|";
		break;
	case OPI_Gui::ElementThemeType::Button:
		key += "Button|";
		break;
	default:
		throw std::invalid_argument("The provided ElementThemeType is invalid.");
	}

	key += theme;
	return key;
}

OPI_Gui::ElementThemeFactory::ElementThemeFactory()
{
}


OPI_Gui::ElementThemeFactory::~ElementThemeFactory()
{
}