#include "Panel.h"

OPI_Gui::Panel::Panel(OPI_Gui::Manager *gui, std::string theme, int x, int y, unsigned short int width, unsigned short int height)
{
	this->theme = theme;
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->loadTheme(theme);
	this->render();
}

OPI_Gui::Panel::~Panel() {};

void OPI_Gui::Panel::loadTheme(std::string theme)
{
	std::string themePath = "IMG/GUI/themes/" + theme + "/";

	//Load template sprite, and then cut out the corners and middle
	SDL_Surface *panelTemplate = OPI_Image::getSurface(themePath + "panel.png");

	//Ensure it loaded
	if (panelTemplate == NULL)
	{
		throw "Panel Template is NULL. Maybe the image is missing from IMG/GUI/themes/<theme_name>/panel.png";
	}

	// Ensure it is a 3x3 "tileset" 
	if (panelTemplate->w % 3 > 0 || panelTemplate->h % 3 > 0)
	{
		throw "Panel template dimensions are incorrect! Width and Height must be divisible by 3.";
	}

	// Determine dimensions automatiacally from the template
	this->tileWidth = panelTemplate->w / 3;
	this->tileHeight = panelTemplate->h / 3;
	SDL_Rect tileClip { 0, 0, tileWidth, tileHeight };

	// Top-Left corner
	corners[0] = OPI_Image::createBlankSurface(tileWidth, tileHeight);
	tileClip.x = 0; 
	tileClip.y = 0;
	SDL_BlitSurface(panelTemplate, &tileClip, corners[0], NULL);

	// Top-Right corner
	corners[1] = OPI_Image::createBlankSurface(tileWidth, tileHeight);
	tileClip.x = tileWidth*2;
	tileClip.y = 0;
	SDL_BlitSurface(panelTemplate, &tileClip, corners[1], NULL);

	// Bottom-Left corner
	corners[2] = OPI_Image::createBlankSurface(tileWidth, tileHeight);
	tileClip.x = 0;
	tileClip.y = tileHeight*2;
	SDL_BlitSurface(panelTemplate, &tileClip, corners[2], NULL);

	// Bottom-Right corner
	corners[3] = OPI_Image::createBlankSurface(tileWidth, tileHeight);
	tileClip.x = tileWidth*2;
	tileClip.y = tileHeight*2;
	SDL_BlitSurface(panelTemplate, &tileClip, corners[3], NULL);

	// Top Edge
	edges[0] = OPI_Image::createBlankSurface(tileWidth, tileHeight);
	tileClip.x = tileWidth;
	tileClip.y = 0;
	SDL_BlitSurface(panelTemplate, &tileClip, edges[0], NULL);

	// Bottom Edge 
	edges[1] = OPI_Image::createBlankSurface(tileWidth, tileHeight);
	tileClip.x = tileWidth;
	tileClip.y = tileHeight*2;
	SDL_BlitSurface(panelTemplate, &tileClip, edges[1], NULL);

	// Left Edge
	edges[2] = OPI_Image::createBlankSurface(tileWidth, tileHeight);
	tileClip.x = 0;
	tileClip.y = tileHeight;
	SDL_BlitSurface(panelTemplate, &tileClip, edges[2], NULL);

	// Right Edge
	edges[3] = OPI_Image::createBlankSurface(tileWidth, tileHeight);
	tileClip.x = tileWidth * 2;
	tileClip.y = tileHeight;
	SDL_BlitSurface(panelTemplate, &tileClip, edges[3], NULL);

	// Filler
	filler = OPI_Image::createBlankSurface(tileWidth, tileHeight);
	tileClip.x = tileWidth;
	tileClip.y = tileHeight;
	SDL_BlitSurface(panelTemplate, &tileClip, filler, NULL);
}

void OPI_Gui::Panel::setWidth(short int width)
{
	if (width < this->tileWidth * 3)
		width = tileWidth * 3;

	this->width = width;
}

void OPI_Gui::Panel::setHeight(short int height)
{
	if (height < this->tileHeight * 3)
		height = tileHeight * 3;

	this->height = height;
}

void OPI_Gui::Panel::render()
{
	this->renderer->render(this);
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
		render();
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

void OPI_Gui::Panel::handleMouseMove(int mouseX, int mouseY)
{
	if (this->isMovable && handleSection_Move(mouseX, mouseY))
	{
		return;
	}

	if (this->isResizable && handleSection_Resize(mouseX, mouseY))
	{
		return;
	}

	if (this->isClosable && handleSection_Close(mouseX, mouseY))
	{
		return;
	}
}

void OPI_Gui::Panel::handleMousePressLeft(int mouseX, int mouseY)
{
	if (this->isMovable && handleSection_Move(mouseX, mouseY))
	{
		isMoving = true;
		moveOriginX = this->x;
		moveOriginY = this->y;
		moveOriginGrabX = mouseX;
		moveOriginGrabY = mouseY;
		return;
	}

	if (this->isResizable && handleSection_Resize(mouseX, mouseY))
	{
		isResizing = true;
		return;
	}

	if (this->isClosable && handleSection_Close(mouseX, mouseY))
	{
		isVisible = false;
		OPI_Gui::Manager::getInstance()->setCursor(CursorType::Normal);
		return;
	}
}

void OPI_Gui::Panel::handleMouseReleaseLeft(int mouseX, int mouseY)
{
	if (this->isMovable && handleSection_Move(mouseX, mouseY))
	{
		isMoving = false;
		return;
	}

	if (this->isResizable && handleSection_Resize(mouseX, mouseY))
	{
		isResizing = false;
		return;
	}
}


void OPI_Gui::Panel::handleMousePressRight(int mouseX, int mouseY)
{

}

void OPI_Gui::Panel::handleMouseReleaseRight(int mouseX, int mouseY)
{

}