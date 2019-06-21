#include "OPI_Gui.h"
#include "OPI_Panel.h"

OPI_Gui::OPI_Gui()
{

}

void OPI_Gui::initCursors(std::string normal, std::string move, std::string resize, std::string hourglass)
{
	// Load cursor images
	SDL_Surface* cursorSurfaceNormal = OPI_Image::getSurface(normal);
	SDL_Surface* cursorSurfaceMove = OPI_Image::getSurface(move);
	SDL_Surface* cursorSurfaceResize = OPI_Image::getSurface(resize);
	SDL_Surface* cursorSurfaceHourglass = OPI_Image::getSurface(hourglass);

	cursorNormal = SDL_CreateColorCursor(cursorSurfaceNormal, 0, 0);
	cursorMove = SDL_CreateColorCursor(cursorSurfaceMove, cursorSurfaceMove->w / 2, cursorSurfaceMove->h / 2);
	cursorResize = SDL_CreateColorCursor(cursorSurfaceResize, cursorSurfaceResize->w/2, cursorSurfaceResize->h/2);
	cursorHourglass = SDL_CreateColorCursor(cursorSurfaceHourglass, cursorSurfaceHourglass->w / 2, cursorSurfaceHourglass->h / 2);

	// Free memory of loaded images
	SDL_FreeSurface(cursorSurfaceNormal);
	SDL_FreeSurface(cursorSurfaceMove);
	SDL_FreeSurface(cursorSurfaceResize);
	SDL_FreeSurface(cursorSurfaceHourglass);

	// Set the default cursor
	this->setCursor(OPI_Gui::CursorType::Normal);
}

void OPI_Gui::addPanel(OPI_Panel *panel)
{
	panels.push_back(panel);
}

void OPI_Gui::setCursor(OPI_Gui::CursorType selectedCursor)
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
	default:break;
	}
}

void OPI_Gui::handleMouseMove(int mouseX, int mouseY)
{
	for (OPI_Panel* panel : this->panels) {
		panel->handleMouseMove(mouseX, mouseY);
	}
}

void OPI_Gui::handleMousePressLeft(int mouseX, int mouseY)
{
	for (OPI_Panel* panel : this->panels) {
		panel->handleMousePressLeft(mouseX, mouseY);
	}
}

void OPI_Gui::handleMousePressRight(int mouseX, int mouseY)
{
	for (OPI_Panel* panel : this->panels) {
		panel->handleMousePressRight(mouseX, mouseY);
	}
}


void OPI_Gui::handleMouseRelease(int mouseX, int mouseY)
{
	for (OPI_Panel* panel : this->panels) {
		panel->handleMouseRelease(mouseX, mouseY);
	}
}