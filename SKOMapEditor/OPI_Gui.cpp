#include "OPI_Gui.h"
#include "OPI_Panel.h"

OPI_Gui::OPI_Gui()
{

}

void OPI_Gui::initCursors(std::string normal, std::string move, std::string resize, std::string hourglass, std::string hand)
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
	this->setCursor(CursorType::Normal);
}

void OPI_Gui::addElement(OPI_Gui_Element *element)
{
	children.push_back(element);
}

void OPI_Gui::setCursor(CursorType selectedCursor)
{
	switch (selectedCursor)
	{
	case CursorType::Normal:
		SDL_SetCursor(cursorNormal);
		break;
	case CursorType::Move:
		SDL_SetCursor(cursorMove);
		break;
	case CursorType::Resize:
		SDL_SetCursor(cursorResize);
		break;
	case CursorType::Hourglass:
		SDL_SetCursor(cursorHourglass);
		break;
	case CursorType::Hand:
		SDL_SetCursor(cursorHand);
		break;
	default:break;
	}
}

void OPI_Gui::handleMouseMove(int mouseX, int mouseY)
{
	for (OPI_Gui_Element* element : this->children) {
		element->handleMouseMove(mouseX, mouseY);
	}
}

void OPI_Gui::handleMousePressLeft(int mouseX, int mouseY)
{
	for (OPI_Gui_Element* element : this->children) {
		element->handleMousePressLeft(mouseX, mouseY);
	}
}

void OPI_Gui::handleMousePressRight(int mouseX, int mouseY)
{
	for (OPI_Gui_Element* element : this->children) {
		element->handleMousePressRight(mouseX, mouseY);
	}
}


void OPI_Gui::handleMouseReleaseLeft(int mouseX, int mouseY)
{
	for (OPI_Gui_Element* element : this->children) {
		element->handleMouseReleaseLeft(mouseX, mouseY);
	}
}

void OPI_Gui::handleMouseReleaseRight(int mouseX, int mouseY)
{
	for (OPI_Gui_Element* element : this->children) {
		element->handleMouseReleaseRight(mouseX, mouseY);
	}
}