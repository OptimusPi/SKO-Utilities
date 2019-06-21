#include "OPI_Panel.h"

OPI_Panel::OPI_Panel(OPI_Gui *gui, std::string theme, int x, int y, unsigned short int width, unsigned short int height)
{
	this->parent = gui;
	this->theme = theme;
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->loadTheme(theme);
	this->render();
}

OPI_Panel::~OPI_Panel() {};

void OPI_Panel::loadTheme(std::string theme)
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
		throw "OPI_Panel template dimensions are incorrect! Width and Height must be divisible by 3.";
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

void OPI_Panel::setWidth(short int width)
{
	if (width < this->tileWidth * 3)
		width = tileWidth * 3;

	this->width = width;
}

void OPI_Panel::setHeight(short int height)
{
	if (height < this->tileHeight * 3)
		height = tileHeight * 3;

	this->height = height;
}

void OPI_Panel::render()
{
	// 
	// Form a blank canvas to draw on.
	// 
	SDL_Surface *panelCanvas = OPI_Image::createBlankSurface(this->width, this->height);

	// Fill panel background 
	for (short x = this->tileWidth; x <= this->width - this->tileWidth; x += this->tileWidth)
	{
		bool edgeCase = false;
		unsigned short fillX = x;
		if (x >= this->width - this->tileWidth * 2)
		{
			fillX = this->width - this->tileWidth * 2;
			edgeCase = true;
		}

		for (short y = this->tileHeight; y <= this->height - this->tileHeight; y += this->tileHeight)
		{
			unsigned short fillY = y;
			if (y >= this->height - this->tileHeight * 2)
			{
				fillY = this->height - this->tileHeight * 2;
				edgeCase = true;
			}

			// Fill in middle from the top left corner without running over the corners or edges
			if (!edgeCase)
			{
				SDL_Rect fillArea = SDL_Rect{ x, y, this->tileWidth, this->tileHeight };
				SDL_BlitSurface(filler, NULL, panelCanvas, &fillArea);
			}

			
			// Fill in middle from the top left corner without running over the corners or edges
			if (edgeCase)
			{
				SDL_Rect fillArea = SDL_Rect{ fillX, fillY, this->tileWidth, this->tileHeight };
				SDL_BlitSurface(filler, NULL, panelCanvas, &fillArea);
			}
		}
	}

	//
	// Draw border on top of the canvas
	//
	// Draw Top and Bottom edges
	for (short x = this->tileWidth; x <= this->width - this->tileWidth; x += this->tileWidth)
	{
		bool edgeCase = false;
		// Fill in gaps for when window width is not divisible by `tileSize`
		if (x > this->width - this->tileWidth * 2)
		{
			x = this->width - this->tileWidth * 2;
			edgeCase = true;
		}

		// Fill in edges from the start without running over the corners
		SDL_Rect topEdge = SDL_Rect{ x, 0, this->tileWidth, this->tileHeight };
		SDL_Rect bottomEdge = SDL_Rect{ x, this->height - this->tileHeight, this->tileWidth, this->tileHeight };

		SDL_BlitSurface(edges[0], NULL, panelCanvas, &topEdge);
		SDL_BlitSurface(edges[1], NULL, panelCanvas, &bottomEdge);

		if (edgeCase) break;
	}

	// Draw Left and Right edges
	for (short y = this->tileHeight; y <= this->height - this->tileHeight; y += this->tileHeight)
	{
		bool edgeCase = false;

		// Fill in gaps for when window width is not divisible by `tileSize`
		if (y > this->height - this->tileHeight * 2)
		{
			y = this->height - this->tileHeight * 2;
			edgeCase = true;
		}

		// Fill in edges from the start without running over the corners
		SDL_Rect leftEdge = SDL_Rect{ 0, y, this->tileWidth, this->tileHeight };
		SDL_Rect rightEdge = SDL_Rect{ this->width - this->tileWidth, y, this->tileWidth, this->tileHeight };

		SDL_BlitSurface(edges[2], NULL, panelCanvas, &leftEdge);
		SDL_BlitSurface(edges[3], NULL, panelCanvas, &rightEdge);

		if (edgeCase) break;
	}


	// Cap off the edges with the corner pieces on top
	SDL_Rect topLeftCorner = SDL_Rect{ 0, 0, this->tileWidth, this->tileHeight };
	SDL_Rect topRightCorner = SDL_Rect{ this->width - this->tileWidth, 0, 0, 0 };
	SDL_Rect bottomLeftCorner = SDL_Rect{ 0, this->height - this->tileHeight, 0, 0 };
	SDL_Rect bottomRightCorner = SDL_Rect{ this->width - this->tileWidth, this->height - this->tileHeight, 0, 0 };
	SDL_BlitSurface(corners[0], NULL, panelCanvas, &topLeftCorner);
	SDL_BlitSurface(corners[1], NULL, panelCanvas, &topRightCorner);
	SDL_BlitSurface(corners[2], NULL, panelCanvas, &bottomLeftCorner);
	SDL_BlitSurface(corners[3], NULL, panelCanvas, &bottomRightCorner);

	// Finally, convert slow SDL_Surface into OpenGL texture for fast drawing.
	delete texture;
	texture = new OPI_Image(panelCanvas);
	SDL_FreeSurface(panelCanvas);
	return;
}

bool OPI_Panel::containsMouse(int mouseX, int mouseY, int x, int y, int w, int h)
{
	// If it's off to one side, it is not contained.
	if (mouseX < x || mouseX > x+w)
		return false;

	// If it's above or below, it is not contained. 
	if (mouseY < y || mouseY > y+h)
		return false;

	return true;
}

bool OPI_Panel::handleResize(int mouseX, int mouseY)
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
	if (containsMouse(mouseX, mouseY, this->x + this->width-24, this->y + this->height-24, 24, 24))
	{
		this->parent->setCursor(OPI_Gui::CursorType::Resize);
		// Signal that event has been handled
		return true;
	}

	// Reset to normal cursor
	this->parent->setCursor(OPI_Gui::CursorType::Normal);

	// Signal to keep processing this event
	return false;
}


bool OPI_Panel::handleMove(int mouseX, int mouseY)
{
	if (this->isMoving)
	{
		setWidth(mouseX - this->x);
		setHeight(mouseY - this->y);
		render();
		// Signal that event has been handled
		return true;
	}

	// Set resize cursor if inside lower-right corner
	if (containsMouse(mouseX, mouseY, this->x , this->y, this->width, 24))
	{
		this->parent->setCursor(OPI_Gui::CursorType::Move);
		// Signal that event has been handled
		return true;
	}

	// Reset to normal cursor
	this->parent->setCursor(OPI_Gui::CursorType::Normal);

	// Signal to keep processing this event
	return false;
}

void OPI_Panel::handleMouseMove(int mouseX, int mouseY)
{
	if (this->isMovable && handleMove(mouseX, mouseY))
	{
		return;
	}

	if (this->isResizable && handleResize(mouseX, mouseY))
	{
		return;
	}
}

void OPI_Panel::handleMousePressLeft(int mouseX, int mouseY)
{

}


void OPI_Panel::handleMousePressRight(int mouseX, int mouseY)
{

}

void OPI_Panel::handleMouseRelease(int mouseX, int mouseY)
{

}