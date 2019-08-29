#include "OPI_GuiElementThemeButtonImage.h"



OPI_Gui::ElementThemeButtonImage::ElementThemeButtonImage()
{

}


OPI_Gui::ElementThemeButtonImage::~ElementThemeButtonImage()
{

}

int OPI_Gui::ElementThemeButtonImage::getMinimumWidth()
{
	return this->texture->width;
}

int OPI_Gui::ElementThemeButtonImage::getMaximumWidth()
{
	return this->texture->width;
}

int OPI_Gui::ElementThemeButtonImage::getMinimumHeight()
{
	return this->texture->height;
}

int OPI_Gui::ElementThemeButtonImage::getMaximumHeight()
{
	return this->texture->height;
}

void OPI_Gui::ElementThemeButtonImage::render(OPI_Gui::Element * element)
{
	this->render((OPI_Gui::Button *)element);
}

void OPI_Gui::ElementThemeButtonImage::render(OPI_Gui::Button * button)
{
	button->setTexture(this->texture);
	return;
}
