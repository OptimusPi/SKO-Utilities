#include "ElementThemeImage.h"



OPI_Gui::OPI_ElementThemeImage::OPI_ElementThemeImage()
{
}


OPI_Gui::OPI_ElementThemeImage::~OPI_ElementThemeImage()
{
}

int OPI_Gui::OPI_ElementThemeImage::getMinimumWidth()
{
	return this->texture->width;
}

int OPI_Gui::OPI_ElementThemeImage::getMinimumHeight()
{
	return this->texture->height;
}

int OPI_Gui::OPI_ElementThemeImage::getMaximumWidth()
{
	return this->texture->width;
}

int OPI_Gui::OPI_ElementThemeImage::getMaximumHeight()
{
	return this->texture->height;
}

void OPI_Gui::OPI_ElementThemeImage::render(OPI_Gui::Element * element)
{
	element->setTexture(this->texture);
}
