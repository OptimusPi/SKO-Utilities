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
	// Forwhen the button is in its default, enabled state
	if (!button->isEnabled)
	{
		button->setTexture(this->textureDisabled);
		return;
	}

	// For when the mouse is pressing down on this button
	if (button->isPressed)
	{
		button->setTexture(this->texturePressed);
		return;
	}

	// For toggle buttons, display as selected when it is on
	if (button->isToggle && button->isToggleOn)
	{
		button->setTexture(this->textureSelected);
		return;
	}

	// For when the mouse hovers over this button
	if (!button->isToggle && button->isSelected)
	{
		button->setTexture(this->textureSelected);
		return;
	}

	// If no other state, just draw regular Button image.
	button->setTexture(this->textureEnabled);
}
