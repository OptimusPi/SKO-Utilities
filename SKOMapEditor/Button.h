#ifndef __OPI_BUTTON_
#define __OPI_BUTTON_

#include <functional>

#include "Element.h"
#include "OPI_Image.h"
#include "TextLabel.h"
#include "ElementThemeFactory.h"
#include "ButtonTheme.h"
#include "OPI_FontManager.h"

#include <string>

namespace OPI_Gui
{
	class Button : public Element
	{
	public:
		Button(std::string themeImage, int x, int y);
		Button(std::string theme, int x, int y, OPI_Text * text);
		virtual ~Button();
		void addCallback(std::function<void()>);
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
		OPI_Gui::ButtonTheme *theme;
		OPI_Image *texture;
		OPI_Text *text;

		std::function<void()> callback;
	};
}
#endif