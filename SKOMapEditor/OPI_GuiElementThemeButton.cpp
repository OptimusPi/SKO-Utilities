#include "OPI_GuiElementThemeButton.h"
#include "OPI_GuiButton.h"

OPI_Gui::ElementThemeButton::ElementThemeButton()
{
}


OPI_Gui::ElementThemeButton::~ElementThemeButton()
{
}

int OPI_Gui::ElementThemeButton::getMinimumWidth()
{
	return this->textureEnabled->width;
}

int OPI_Gui::ElementThemeButton::getMinimumHeight()
{
	return this->textureEnabled->height;
}

int OPI_Gui::ElementThemeButton::getMaximumWidth()
{
	return this->textureEnabled->width;
}

int OPI_Gui::ElementThemeButton::getMaximumHeight()
{
	return this->textureEnabled->height;
}

void OPI_Gui::ElementThemeButton::render(OPI_Gui::Element * element)
{
	return;
}

void OPI_Gui::ElementThemeButton::render(OPI_Gui::Button * button)
{

	if (!button->isEnabled)
	{
		button->setTexture(this->textureDisabled);
		return;
	}

	if (button->isPressed)
	{
		button->setTexture(this->texturePressed);
		return;
	}

	if (button->isSelected)
	{
		button->setTexture(this->textureSelected);
		return;
	}


	// If no other state, just draw regular Button image.
	button->setTexture(this->textureEnabled);
}
