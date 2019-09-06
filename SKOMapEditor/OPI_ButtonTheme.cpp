#include "ButtonTheme.h"
#include "Button.h"

OPI_Gui::ButtonTheme::ButtonTheme()
{
}

OPI_Gui::ButtonTheme::~ButtonTheme()
{
}

int OPI_Gui::ButtonTheme::getMinimumWidth()
{
	return this->textureEnabled->width;
}

int OPI_Gui::ButtonTheme::getMinimumHeight()
{
	return this->textureEnabled->height;
}

int OPI_Gui::ButtonTheme::getMaximumWidth()
{
	return this->textureEnabled->width;
}

int OPI_Gui::ButtonTheme::getMaximumHeight()
{
	return this->textureEnabled->height;
}

void OPI_Gui::ButtonTheme::render(OPI_Gui::Button * button)
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
