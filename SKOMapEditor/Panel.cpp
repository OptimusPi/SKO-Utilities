#include "Panel.h"


OPI_Gui::Panel::Panel(OPI_Gui::ElementThemeType type, std::string theme, int x, int y, int width, int height)
{
	this->theme = OPI_Gui::ThemeLoader::GetTheme(type, theme);
	this->x = x;
	this->y = y;
	this->width = width < this->theme->getMinimumWidth() ? this->theme->getMinimumWidth() : width;
	this->height = height < this->theme->getMinimumHeight() ? this->theme->getMinimumHeight() : height;
	this->theme->render(this);
}

OPI_Gui::Panel::~Panel() {};

void OPI_Gui::Panel::setWidth(short int width)
{
	int minimumWidth = this->theme->getMinimumWidth();
	if (width < minimumWidth)
		width = minimumWidth;

	this->width = width;
}

void OPI_Gui::Panel::setHeight(short int height)
{
	int minimumHeight = this->theme->getMinimumHeight();
	if (height < minimumHeight)
		height = minimumHeight;

	this->height = height;
}

bool OPI_Gui::Panel::containsMouse(int mouseX, int mouseY, int x, int y, int w, int h)
{
	// If it's off to one side, it is not contained.
	if (mouseX < x || mouseX > x+w)
		return false;

	// If it's above or below, it is not contained. 
	if (mouseY < y || mouseY > y+h)
		return false;

	return true;
}

// TODO - allow customizable moveable grab area
bool OPI_Gui::Panel::movableContainsMouse(int mouseX, int mouseY)
{
	//if (moveableGrabArea == NULL)
	//	return false;

	return containsMouse(mouseX, mouseY, this->x, this->y, this->width - 24, 24);
}

// TODO - allow customizable resizable grab area
bool OPI_Gui::Panel::resizableContainsMouse(int mouseX, int mouseY)
{
	//if (resizableGrabArea == NULL)
	//	return false;

	return containsMouse(mouseX, mouseY, this->x + this->width - 24, this->y + this->height - 24, 24, 24);
}

// TODO - allow customizable closable grab area
bool OPI_Gui::Panel::closableContainsMouse(int mouseX, int mouseY)
{
	//if (closableGrabArea == NULL)
	//	return false;

	return containsMouse(mouseX, mouseY, this->x + this->width - 24, this->y, 24, 24);
}

void OPI_Gui::Panel::setCursor(CursorType cursor)
{
	OPI_Gui::Manager::getInstance()->setCursor(cursor);
}

bool OPI_Gui::Panel::handleSection_Move(int mouseX, int mouseY)
{
	if (this->isMoving)
	{
		this->x = moveOriginX + mouseX - moveOriginGrabX;
		this->y = moveOriginY + mouseY - moveOriginGrabY;

		// Signal that event has been handled
		return true;
	}

	// Set resize cursor if inside lower-right corner
	if (movableContainsMouse(mouseX, mouseY))
	{
		OPI_Gui::Manager::getInstance()->setCursor(CursorType::Move);
		// Signal that event has been handled
		return true;
	}

	// Reset to normal cursor
	if (!this->isResizing)
		OPI_Gui::Manager::getInstance()->setCursor(CursorType::Normal);

	// Signal to keep processing this event
	return false;
}

bool OPI_Gui::Panel::handleSection_Resize(int mouseX, int mouseY)
{
	if (this->isResizing)
	{
		setWidth(mouseX - this->x);
		setHeight(mouseY - this->y);
		this->theme->render(this);
		// Signal that event has been handled
		return true;
	}

	// Set resize cursor if inside lower-right corner
	if (resizableContainsMouse(mouseX, mouseY))
	{
		OPI_Gui::Manager::getInstance()->setCursor(CursorType::Resize);
		// Signal that event has been handled
		return true;
	}

	// Reset to normal cursor
	if (!this->isMoving)
		OPI_Gui::Manager::getInstance()->setCursor(CursorType::Normal);

	// Signal to keep processing this event
	return false;
}


bool OPI_Gui::Panel::handleSection_Close(int mouseX, int mouseY)
{
	// Set resize cursor if inside lower-right corner
	if (closableContainsMouse(mouseX, mouseY))
	{
		OPI_Gui::Manager::getInstance()->setCursor(CursorType::Hand);
		// Signal that event has been handled
		return true;
	}

	// Signal to keep processing this event
	return false;
}

bool OPI_Gui::Panel::handleMouseMove(int mouseX, int mouseY)
{
	if (this->isMovable && handleSection_Move(mouseX, mouseY))
	{
		return true;
	}

	if (this->isResizable && handleSection_Resize(mouseX, mouseY))
	{
		return true;
	}

	if (this->isClosable && handleSection_Close(mouseX, mouseY))
	{
		return true;
	}

	return false;
}

bool OPI_Gui::Panel::handleMousePressLeft(int mouseX, int mouseY)
{
	if (this->isMovable && handleSection_Move(mouseX, mouseY))
	{
		isMoving = true;
		moveOriginX = this->x;
		moveOriginY = this->y;
		moveOriginGrabX = mouseX;
		moveOriginGrabY = mouseY;
		return true;
	}

	if (this->isResizable && handleSection_Resize(mouseX, mouseY))
	{
		isResizing = true;
		return true;
	}

	if (this->isClosable && handleSection_Close(mouseX, mouseY))
	{
		isVisible = false;
		OPI_Gui::Manager::getInstance()->setCursor(CursorType::Normal);
		return true;
	}

	return false;
}

bool OPI_Gui::Panel::handleMouseReleaseLeft(int mouseX, int mouseY)
{
	if (this->isMovable && handleSection_Move(mouseX, mouseY))
	{
		isMoving = false;
		return true;
	}

	if (this->isResizable && handleSection_Resize(mouseX, mouseY))
	{
		isResizing = false;
		return true;
	}

	return false;
}


bool OPI_Gui::Panel::handleMousePressRight(int mouseX, int mouseY)
{
	return false;
}

bool OPI_Gui::Panel::handleMouseReleaseRight(int mouseX, int mouseY)
{
	return false;
}