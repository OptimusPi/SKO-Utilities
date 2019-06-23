#include "OPI_Button.h"


OPI_Button::OPI_Button(std::string theme, int x, int y)
{
	loadTheme(theme);
	this->x = x;
	this->y = y;
	this->width = textureEnabled->width;
	this->height = textureEnabled->height;
}

OPI_Button::~OPI_Button()
{

}

void OPI_Button::addCallback(std::function<void(void)> callback)
{
	this->callback = callback;
}

void OPI_Button::loadTheme(std::string theme)
{
	std::string path = "IMG/GUI/themes/" + theme + "/";
	this->textureEnabled = new OPI_Image(path + "enabled.png");
	this->textureDisabled = new OPI_Image(path + "disabled.png");
	this->texturePressed = new OPI_Image(path + "pressed.png");	
	this->textureSelected = new OPI_Image(path + "selected.png");
}
