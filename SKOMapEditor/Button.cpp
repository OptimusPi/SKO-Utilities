#include "Button.h"

OPI_Gui::Button::Button(std::string theme, int x, int y)
{
	this->theme = (OPI_Gui::ElementThemeButton*)(OPI_Gui::ThemeLoader::GetTheme(OPI_Gui::ElementThemeType::Button, theme));
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

void OPI_Gui::Button::setCursor(OPI_Gui::CursorType cursor)
{
	OPI_Gui::Manager::getInstance()->setCursor(cursor);
}

bool OPI_Gui::Button::clickableContainsMouse(int mouseX, int mouseY)
{
	if (mouseX < this->x || mouseX > this->x + this->width)
		return false;
	if (mouseY < this->y || mouseY > this->y + this->height)
		return false;

	return true;
}

bool OPI_Gui::Button::handleMouseMove(int mouseX, int mouseY)
{
	if (!this->isEnabled)
		return false;

	if (clickableContainsMouse(mouseX, mouseY))
	{
		this->isSelected = true;
		this->theme->render(this);
		setCursor(OPI_Gui::CursorType::Hand);
		return true;
	}
	else
	{
		if (this->isSelected)
		{
			setCursor(OPI_Gui::CursorType::Normal);
			this->isPressed = false;
			this->isSelected = false;
			this->theme->render(this);
		}
	}
	return false;
}

bool OPI_Gui::Button::handleMousePressLeft(int mouseX, int mouseY)
{
	if (!this->isEnabled)
		return false;

	if (clickableContainsMouse(mouseX, mouseY))
	{
		this->isPressed = true;
		this->theme->render(this);
		setCursor(OPI_Gui::CursorType::Hand);
		return true;
	}
	return false;
}

bool OPI_Gui::Button::handleMouseReleaseLeft(int mouseX, int mouseY)
{
	if (!this->isEnabled)
		return false;

	if (clickableContainsMouse(mouseX, mouseY))
	{
		this->isPressed = false;
		this->theme->render(this);

		if (this->callback == nullptr)
			return false;

		this->callback();
		return true;
	}
	else
	{
		if (this->isPressed)
		{
			this->isPressed = false;
			this->isSelected = false;
			this->theme->render(this);
			return true;
		}
	}

	return false;
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