#include "OPI_GuiButtonThemeImageSet.h"
#include "OPI_GuiButton.h"

OPI_Gui::ButtonThemeImageSet::ButtonThemeImageSet()
{
}

OPI_Gui::ButtonThemeImageSet::~ButtonThemeImageSet()
{
}

int OPI_Gui::ButtonThemeImageSet::getMinimumWidth()
{
	return this->textureEnabled->width;
}

int OPI_Gui::ButtonThemeImageSet::getMinimumHeight()
{
	return this->textureEnabled->height;
}

int OPI_Gui::ButtonThemeImageSet::getMaximumWidth()
{
	return this->textureEnabled->width;
}

int OPI_Gui::ButtonThemeImageSet::getMaximumHeight()
{
	return this->textureEnabled->height;
}

void OPI_Gui::ButtonThemeImageSet::render(OPI_Gui::Button * button)
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
