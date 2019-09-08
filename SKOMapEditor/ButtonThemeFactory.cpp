#include <typeinfo>

#include "ButtonThemeFactory.h"
#include "ElementThemeGridRect.h"
#include "ElementThemeImage.h"
#include "ButtonTheme.h"

/// Singleton instance
OPI_Gui::ButtonThemeFactory * OPI_Gui::ButtonThemeFactory::instance;
OPI_Gui::ButtonThemeFactory * OPI_Gui::ButtonThemeFactory::getInstance()
{
	if (!OPI_Gui::ButtonThemeFactory::instance)
	{
		OPI_Gui::ButtonThemeFactory::instance = new OPI_Gui::ButtonThemeFactory;
	}

	return OPI_Gui::ButtonThemeFactory::instance;
}

OPI_Gui::ButtonTheme * OPI_Gui::ButtonThemeFactory::GetTheme(OPI_Gui::ElementThemeType type, std::string theme)
{
	return 	getInstance()->getTheme(type, theme);
}

OPI_Gui::ButtonTheme * OPI_Gui::ButtonThemeFactory::getTheme(OPI_Gui::ElementThemeType type, std::string theme)
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
		case ElementThemeType::Button:
			this->loadTheme_Button(theme);
			break;
		case ElementThemeType::ButtonIcon:
			this->loadTheme_ButtonIcon(theme);
			break;
		default:
			throw std::logic_error("Invalid type provided to OPI_Gui::ButtonThemeFactory!");
		}
	}

	// Return Element Theme from memory cache
	return this->themes.at(key);
}

void OPI_Gui::ButtonThemeFactory::loadTheme_Button(std::string theme)
{
	OPI_Gui::ButtonTheme *elementThemeButton = new OPI_Gui::ButtonTheme();
	std::string path = "IMG/GUI/themes/button/" + theme + "/";
	elementThemeButton->textureEnabled = new OPI_Image(path + "enabled.png");
	elementThemeButton->textureDisabled = new OPI_Image(path + "disabled.png");
	elementThemeButton->texturePressed = new OPI_Image(path + "pressed.png");
	elementThemeButton->textureSelected = new OPI_Image(path + "selected.png");

	//insert into cache
	std::string key = generateKey(OPI_Gui::ElementThemeType::Button, theme);
	this->themes.insert({ {key, elementThemeButton} });
}

void OPI_Gui::ButtonThemeFactory::loadTheme_ButtonIcon(std::string themeImage)
{
	OPI_Gui::ButtonTheme *elementThemeButtonIcon = new OPI_Gui::ButtonTheme();
	std::string path = "IMG/GUI/themes/button_images/" + themeImage;
	elementThemeButtonIcon->textureEnabled = new OPI_Image(path + ".png");
	//TODO 4 styates for icon buttons

	//insert into cache
	std::string key = generateKey(OPI_Gui::ElementThemeType::ButtonIcon, themeImage);
	this->themes.insert({ {key, elementThemeButtonIcon} });
}

std::string OPI_Gui::ButtonThemeFactory::generateKey(OPI_Gui::ElementThemeType type, std::string theme)
{
	std::string key = "";

	switch (type)
	{
	case OPI_Gui::ElementThemeType::Blank:
		return "";
	case OPI_Gui::ElementThemeType::Button:
		key += "Button|";
		break;
	case OPI_Gui::ElementThemeType::ButtonIcon:
		key += "ButtonIcon|";
		break;
	default:
		throw std::invalid_argument("The provided ElementThemeType is invalid.");
	}

	key += theme;
	return key;
}

OPI_Gui::ButtonThemeFactory::ButtonThemeFactory()
{
}


OPI_Gui::ButtonThemeFactory::~ButtonThemeFactory()
{
}
