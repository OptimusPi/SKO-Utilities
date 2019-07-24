#ifndef __OPI_BUTTON_
#define __OPI_BUTTON_

#include <functional>

#include "OPI_GuiElement.h"
#include "OPI_Image.h"
#include "OPI_Text.h"
#include "OPI_GuiThemeLoader.h"
#include "OPI_GuiManager.h"
#include "OPI_GuiElementThemeButton.h"

#include <string>

namespace OPI_Gui
{
	class Button : public Element
	{
	public:
		Button(std::string theme, int x, int y);
		virtual ~Button();
		void addCallback(std::function<void(void)> callback);
		bool isPressed = false;
		bool isSelected = false;
		bool isEnabled = true;

		// Input handlers
		bool handleMouseMove(int mouseX, int mouseY);
		bool handleMousePressLeft(int mouseX, int mouseY);
		bool handleMousePressRight(int mouseX, int mouseY);
		bool handleMouseReleaseRight(int mouseX, int mouseY);
		bool handleMouseReleaseLeft(int mouseX, int mouseY);
	private:
		bool clickableContainsMouse(int mouseX, int mouseY);
		void setCursor(CursorType cursor);
		OPI_Gui::ElementThemeButton *theme;
		OPI_Image *texture;
		OPI_Text *text;

		std::function<void(void)> callback;
	};
}
#endif