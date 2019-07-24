#include "OPI_GuiElementThemeImage.h"



OPI_Gui::ElementThemeImage::ElementThemeImage()
{
}


OPI_Gui::ElementThemeImage::~ElementThemeImage()
{
}

int OPI_Gui::ElementThemeImage::getMinimumWidth()
{
	return this->texture->width;
}

int OPI_Gui::ElementThemeImage::getMinimumHeight()
{
	return this->texture->height;
}

int OPI_Gui::ElementThemeImage::getMaximumWidth()
{
	return this->texture->width;
}

int OPI_Gui::ElementThemeImage::getMaximumHeight()
{
	return this->texture->height;
}

void OPI_Gui::ElementThemeImage::render(OPI_Gui::Element * element)
{
	element->setTexture(this->texture);
}
