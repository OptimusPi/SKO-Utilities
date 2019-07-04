#include "Manager.h"
#include "Panel.h"


///Singleton instance
OPI_Gui::Manager * OPI_Gui::Manager::instance;
OPI_Gui::Manager * OPI_Gui::Manager::getInstance()
{
	if (!OPI_Gui::Manager::instance) 
	{
		OPI_Gui::Manager::instance = new OPI_Gui::Manager;
	}

	return OPI_Gui::Manager::instance;
}

void OPI_Gui::Manager::initCursors(std::string normal, std::string move, std::string resize, std::string hourglass, std::string hand)
{
	// Load cursor images
	SDL_Surface* cursorSurfaceNormal = OPI_Image::getSurface(normal);
	SDL_Surface* cursorSurfaceMove = OPI_Image::getSurface(move);
	SDL_Surface* cursorSurfaceResize = OPI_Image::getSurface(resize);
	SDL_Surface* cursorSurfaceHourglass = OPI_Image::getSurface(hourglass);
	SDL_Surface* cursorSurfaceHand = OPI_Image::getSurface(hand);

	cursorNormal = SDL_CreateColorCursor(cursorSurfaceNormal, 0, 0);
	cursorMove = SDL_CreateColorCursor(cursorSurfaceMove, cursorSurfaceMove->w / 2, cursorSurfaceMove->h / 2);
	cursorResize = SDL_CreateColorCursor(cursorSurfaceResize, cursorSurfaceResize->w / 2, cursorSurfaceResize->h / 2);
	cursorHourglass = SDL_CreateColorCursor(cursorSurfaceHourglass, cursorSurfaceHourglass->w / 2, cursorSurfaceHourglass->h / 2);
	cursorHand = SDL_CreateColorCursor(cursorSurfaceHand, cursorSurfaceHand->w / 3, 0);

	// Free memory of loaded images
	SDL_FreeSurface(cursorSurfaceNormal);
	SDL_FreeSurface(cursorSurfaceMove);
	SDL_FreeSurface(cursorSurfaceResize);
	SDL_FreeSurface(cursorSurfaceHourglass);
	SDL_FreeSurface(cursorSurfaceHand);

	// Set the default cursor
	this->setCursor(OPI_Gui::CursorType::Normal);
}

void OPI_Gui::Manager::addElement(OPI_Gui::Element *element)
{
	children.push_back(element);
}

void OPI_Gui::Manager::setCursor(OPI_Gui::CursorType selectedCursor)
{
	switch (selectedCursor)
	{
	case OPI_Gui::CursorType::Normal:
		SDL_SetCursor(cursorNormal);
		break;
	case OPI_Gui::CursorType::Move:
		SDL_SetCursor(cursorMove);
		break;
	case OPI_Gui::CursorType::Resize:
		SDL_SetCursor(cursorResize);
		break;
	case OPI_Gui::CursorType::Hourglass:
		SDL_SetCursor(cursorHourglass);
		break;
	case OPI_Gui::CursorType::Hand:
		SDL_SetCursor(cursorHand);
		break;
	default:break;
	}
}

void OPI_Gui::Manager::handleMouseMove(int mouseX, int mouseY)
{
	for (auto i = this->children.rbegin(); i != this->children.rend(); i++)
	{
		OPI_Gui::Element *element = *i;
		if (element->handleMouseMove(mouseX, mouseY))
			break;
	}
}

void OPI_Gui::Manager::handleMousePressLeft(int mouseX, int mouseY)
{
	for (auto i = this->children.rbegin(); i != this->children.rend(); i++)
	{
		OPI_Gui::Element *element = *i;
		if (element->handleMousePressLeft(mouseX, mouseY))
			break;
	}
}

void OPI_Gui::Manager::handleMousePressRight(int mouseX, int mouseY)
{
	for (auto i = this->children.rbegin(); i != this->children.rend(); i++)
	{
		OPI_Gui::Element *element = *i;
		if (element->handleMousePressRight(mouseX, mouseY))
			break;
	}
}


void OPI_Gui::Manager::handleMouseReleaseLeft(int mouseX, int mouseY)
{
	for (auto i = this->children.rbegin(); i != this->children.rend(); i++)
	{
		OPI_Gui::Element *element = *i;
		if (element->handleMouseReleaseLeft(mouseX, mouseY))
			break;
	}
}

void OPI_Gui::Manager::handleMouseReleaseRight(int mouseX, int mouseY)
{
	for (auto i = this->children.rbegin(); i != this->children.rend(); i++)
	{
		OPI_Gui::Element *element = *i;
		if (element->handleMouseReleaseRight(mouseX, mouseY))
			break;
	}
}