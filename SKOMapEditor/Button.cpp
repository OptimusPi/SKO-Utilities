#include "Button.h"

OPI_Gui::Button::Button(OPI_Gui::ElementThemeType type, std::string theme, int x, int y)
{
	this->theme = OPI_Gui::ThemeLoader::GetTheme(type, theme);
	this->x = x;
	this->y = y;
	this->width = width < this->theme->getMinimumWidth() ? this->theme->getMinimumWidth() : width;
	this->height = height < this->theme->getMinimumHeight() ? this->theme->getMinimumHeight() : height;
	this->width = this->width > this->theme->getMaximumWidth() && this->theme->getMaximumWidth() > 0 ? this->theme->getMinimumWidth() : this->width;
	this->height = this->height > this->theme->getMaximumHeight() && this->theme->getMaximumHeight() > 0 ? this->theme->getMinimumHeight() : this->height;
	this->theme->render(this);
}

void OPI_Gui::Button::addCallback(std::function<void(void)> callback)
{
	this->callback = callback;
}


bool OPI_Gui::Button::handleMouseMove(int mouseX, int mouseY)
{
	// TODO  - set the button to selected state.
	return false;
}

bool OPI_Gui::Button::handleMousePressLeft(int mouseX, int mouseY)
{
	// TODO  - set the button to pressed state.
	return false;
}

bool OPI_Gui::Button::handleMouseReleaseLeft(int mouseX, int mouseY)
{
	// TODO press the button and run the action
	if (this->callback == nullptr)
		return true;

	this->callback();

	return true;
}


bool OPI_Gui::Button::handleMousePressRight(int mouseX, int mouseY)
{
	return false;
}

bool OPI_Gui::Button::handleMouseReleaseRight(int mouseX, int mouseY)
{
	return false;
}

OPI_Gui::Button::~Button()
{

}