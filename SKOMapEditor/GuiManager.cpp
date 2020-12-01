#include "GuiManager.h"



// TODO - get rid of Global.h
#include "Global.h"


///Singleton instance
OPI_Gui::GuiManager * OPI_Gui::GuiManager::instance;
void OPI_Gui::GuiManager::create(OPI_Renderer *renderer)
{
	// Create instance
	OPI_Gui::GuiManager *instance = new OPI_Gui::GuiManager;
	OPI_Gui::GuiManager::instance = instance;
	instance->renderer = renderer;

	// Store current window width and height
	SDL_GetWindowSize(instance->renderer->window, &instance->screenWidth, &instance->screenHeight);
}

void OPI_Gui::GuiManager::initCursors(std::string normal, std::string move, std::string resize, std::string hourglass, std::string hand)
{
	// Load cursor images
	SDL_Surface* cursorSurfaceNormal = OPI_Image::getSurface(normal);
	SDL_Surface* cursorSurfaceMove = OPI_Image::getSurface(move);
	SDL_Surface* cursorSurfaceResize = OPI_Image::getSurface(resize);
	SDL_Surface* cursorSurfaceHourglass = OPI_Image::getSurface(hourglass);
	SDL_Surface* cursorSurfaceHand = OPI_Image::getSurface(hand);

	instance->cursorNormal = SDL_CreateColorCursor(cursorSurfaceNormal, 0, 0);
	instance->cursorMove = SDL_CreateColorCursor(cursorSurfaceMove, cursorSurfaceMove->w / 2, cursorSurfaceMove->h / 2);
	instance->cursorResize = SDL_CreateColorCursor(cursorSurfaceResize, cursorSurfaceResize->w / 2, cursorSurfaceResize->h / 2);
	instance->cursorHourglass = SDL_CreateColorCursor(cursorSurfaceHourglass, cursorSurfaceHourglass->w / 2, cursorSurfaceHourglass->h / 2);
	instance->cursorHand = SDL_CreateColorCursor(cursorSurfaceHand, cursorSurfaceHand->w / 3, 0);

	// Free memory of loaded images
	SDL_FreeSurface(cursorSurfaceNormal);
	SDL_FreeSurface(cursorSurfaceMove);
	SDL_FreeSurface(cursorSurfaceResize);
	SDL_FreeSurface(cursorSurfaceHourglass);
	SDL_FreeSurface(cursorSurfaceHand);

	// Set the default cursor
	instance->setCursor(OPI_Gui::CursorType::Normal);
}

void OPI_Gui::GuiManager::addElement(OPI_Gui::Element *element)
{
	instance->children.push_back(element);
}

void OPI_Gui::GuiManager::setCursor(OPI_Gui::CursorType selectedCursor)
{
	switch (selectedCursor)
	{
	case OPI_Gui::CursorType::Normal:
		SDL_SetCursor(instance->cursorNormal);
		break;
	case OPI_Gui::CursorType::Move:
		SDL_SetCursor(instance->cursorMove);
		break;
	case OPI_Gui::CursorType::Resize:
		SDL_SetCursor(instance->cursorResize);
		break;
	case OPI_Gui::CursorType::Hourglass:
		SDL_SetCursor(instance->cursorHourglass);
		break;
	case OPI_Gui::CursorType::Hand:
		SDL_SetCursor(instance->cursorHand);
		break;
	default:break;
	}
}

bool OPI_Gui::GuiManager::handleMouseMove_InteractingElements(int mouseX, int mouseY)
{
	for (auto i = instance->children.rbegin(); i != instance->children.rend(); ++i)
	{
		OPI_Gui::Element *element = *i;
		if (element->isInteracting() && element->handleMouseMove(mouseX, mouseY))
			return true;
	}
	return false;
}

bool OPI_Gui::GuiManager::handleMouseMove(int mouseX, int mouseY)
{
	bool eventHandled = false;

	// Allow elements to pass over, under, and across each other when dragging without losing mouse focus
	if (instance->handleMouseMove_InteractingElements(mouseX, mouseY))
		return true;

	for (auto i = instance->children.rbegin(); i != instance->children.rend(); ++i)
	{
		OPI_Gui::Element *element = *i;
		if (element->handleMouseMove(mouseX, mouseY))
			eventHandled = true;
	}

	return eventHandled;
}

bool OPI_Gui::GuiManager::handleMousePressLeft(int mouseX, int mouseY)
{
	bool eventHandled = false;
	for (auto i = instance->children.rbegin(); i != instance->children.rend(); ++i)
	{
		OPI_Gui::Element *element = *i;
		if (element->handleMousePressLeft(mouseX, mouseY))
			eventHandled = true;
	}
	return eventHandled;
}

bool OPI_Gui::GuiManager::handleMousePressRight(int mouseX, int mouseY)
{
	bool eventHandled = false;
	for (auto i = instance->children.rbegin(); i != instance->children.rend(); ++i)
	{
		OPI_Gui::Element *element = *i;
		if (element->handleMousePressRight(mouseX, mouseY))
			eventHandled = true;
	}
	return eventHandled;
}

bool OPI_Gui::GuiManager::handleMouseReleaseLeft(int mouseX, int mouseY)
{
	bool eventHandled = false;
	for (auto i = instance->children.rbegin(); i != instance->children.rend(); ++i)
	{
		OPI_Gui::Element *element = *i;
		if (element->handleMouseReleaseLeft(mouseX, mouseY))
			eventHandled = true;
	}

	return eventHandled;
}

bool OPI_Gui::GuiManager::handleMouseReleaseRight(int mouseX, int mouseY)
{
	bool eventHandled = false;
	for (auto i = instance->children.rbegin(); i != instance->children.rend(); ++i)
	{
		OPI_Gui::Element *element = *i;
		if (element->handleMouseReleaseRight(mouseX, mouseY))
			eventHandled = true;
	}
	return eventHandled;
}

int OPI_Gui::GuiManager::getScreenWidth()
{
	return instance->screenWidth;
}

int OPI_Gui::GuiManager::getScreenHeight()
{
	return instance->screenHeight;
}

void OPI_Gui::GuiManager::drawElement(int x, int y, OPI_Gui::Element * element)
{
	SDL_Rect selection;
	selection.x = 0;
	selection.y = 0;
	selection.w = element->getTexture()->width;
	selection.h = element->getTexture()->height;

	renderer->drawImage(x + element->x, y + element->y, element->getTexture(), selection, 0.f);
	for (OPI_Gui::Element* child : element->children) 
	{
		if (child->isVisible)
		{
			drawElement(x + element->x, y + element->y, child);
		}
	}
}

OPI_Gui::GuiManager * OPI_Gui::GuiManager::getInstance()
{
	return OPI_Gui::GuiManager::instance;
}
