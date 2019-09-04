#include "OPI_GuiButtonThemeIcon.h"



OPI_Gui::ButtonThemeIcon::ButtonThemeIcon()
{

}


OPI_Gui::ButtonThemeIcon::~ButtonThemeIcon()
{

}

int OPI_Gui::ButtonThemeIcon::getMinimumWidth()
{
	return this->textureEnabled->width;
}

int OPI_Gui::ButtonThemeIcon::getMaximumWidth()
{
	return this->textureEnabled->width;
}

int OPI_Gui::ButtonThemeIcon::getMinimumHeight()
{
	return this->textureEnabled->height;
}

int OPI_Gui::ButtonThemeIcon::getMaximumHeight()
{
	return this->textureEnabled->height;
}

void OPI_Gui::ButtonThemeIcon::render(OPI_Gui::Element * element)
{
	this->render((OPI_Gui::Button *)element);
}

void OPI_Gui::ButtonThemeIcon::render(OPI_Gui::Button * button)
{
	button->setTexture(this->texture);
	return;
}
