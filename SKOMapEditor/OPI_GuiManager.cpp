#include "OPI_GuiManager.h"
#include "OPI_GuiPanel.h"


///Singleton instance
OPI_Gui::GuiManager * OPI_Gui::GuiManager::instance;
OPI_Gui::GuiManager * OPI_Gui::GuiManager::getInstance()
{
	if (!OPI_Gui::GuiManager::instance)
	{
		OPI_Gui::GuiManager::instance = new OPI_Gui::GuiManager;
	}

	return OPI_Gui::GuiManager::instance;
}

void OPI_Gui::GuiManager::initCursors(std::string normal, std::string move, std::string resize, std::string hourglass, std::string hand)
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

void OPI_Gui::GuiManager::addElement(OPI_Gui::Element *element)
{
	children.push_back(element);
}

void OPI_Gui::GuiManager::setCursor(OPI_Gui::CursorType selectedCursor)
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



bool OPI_Gui::GuiManager::handleMouseMove_InteractingElements(int mouseX, int mouseY)
{
	for (auto i = this->children.rbegin(); i != this->children.rend(); i++)
	{
		OPI_Gui::Element *element = *i;
		if (element->isInteracting() && element->handleMouseMove(mouseX, mouseY))
			return true;
	}
	return false;
}

void OPI_Gui::GuiManager::handleMouseMove(int mouseX, int mouseY)
{
	// Allow elements to pass over, under, and across each other when dragging without losing mouse focus
	if (handleMouseMove_InteractingElements(mouseX, mouseY))
		return;

	for (auto i = this->children.rbegin(); i != this->children.rend(); i++)
	{
		OPI_Gui::Element *element = *i;
		if (element->handleMouseMove(mouseX, mouseY))
			break;
	}
}

void OPI_Gui::GuiManager::handleMousePressLeft(int mouseX, int mouseY)
{
	for (auto i = this->children.rbegin(); i != this->children.rend(); i++)
	{
		OPI_Gui::Element *element = *i;
		if (element->handleMousePressLeft(mouseX, mouseY))
			break;
	}
}

void OPI_Gui::GuiManager::handleMousePressRight(int mouseX, int mouseY)
{
	for (auto i = this->children.rbegin(); i != this->children.rend(); i++)
	{
		OPI_Gui::Element *element = *i;
		if (element->handleMousePressRight(mouseX, mouseY))
			break;
	}
}

//void OPI_Gui::Manager::handleMouseReleaseLeft(OPI_Gui::Element *element, int mouseX, int mouseY)
//{
//	for (auto i = element->children.rbegin(); i != element->children.rend(); i++)
//	{
//		OPI_Gui::Element *child = *i;
//		handleMouseReleaseLeft(child, mouseX, mouseY);
//		if (element->handleMouseReleaseLeft(mouseX, mouseY))
//			break;
//	}
//}

void OPI_Gui::GuiManager::handleMouseReleaseLeft(int mouseX, int mouseY)
{
	for (auto i = this->children.rbegin(); i != this->children.rend(); i++)
	{
		OPI_Gui::Element *element = *i;
		if (element->handleMouseReleaseLeft(mouseX, mouseY))
			break;
	}
}

void OPI_Gui::GuiManager::handleMouseReleaseRight(int mouseX, int mouseY)
{
	for (auto i = this->children.rbegin(); i != this->children.rend(); i++)
	{
		OPI_Gui::Element *element = *i;
		if (element->handleMouseReleaseRight(mouseX, mouseY))
			break;
	}
}

int OPI_Gui::GuiManager::getScreenWidth()
{
	SDL_DisplayMode DM;
	SDL_GetCurrentDisplayMode(0, &DM);
	return DM.w;
}

int OPI_Gui::GuiManager::getScreenHeight()
{
	SDL_DisplayMode DM;
	SDL_GetCurrentDisplayMode(0, &DM);
	return DM.h;
}
