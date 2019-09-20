#ifndef __OPI_BUTTON_
#define __OPI_BUTTON_

#include <functional>

#include "Element.h"
#include "OPI_Image.h"
#include "TextLabel.h"
#include "ButtonThemeFactory.h"
#include "FontManager.h"
#include "GuiManager.h"
#include "ElementThemeType.h"
#include "TextComponent.h";

#include <string>

namespace OPI_Gui
{
	class ButtonTheme;
	class ButtonToggleGroup;

	class Button : public Element
	{
	public:
		Button(std::string themeImage, int x, int y);
		Button(std::string theme, int x, int y, OPI_Text::TextComponent * text);
		~Button();
		void addCallback(std::function<void()>);
		bool isPressed = false;
		bool isSelected = false;
		bool isEnabled = true;
		bool isToggle = false;
		bool isToggleOn = false;
		ButtonToggleGroup *toggleGroup;
		void toggleOff();

		// Input handlers
		bool handleMouseMove(int mouseX, int mouseY);
		bool handleMousePressLeft(int mouseX, int mouseY);
		bool handleMousePressRight(int mouseX, int mouseY);
		bool handleMouseReleaseRight(int mouseX, int mouseY);
		bool handleMouseReleaseLeft(int mouseX, int mouseY);


	private:
		bool clickableContainsMouse(int mouseX, int mouseY);
		void setCursor(CursorType cursor);
		OPI_Image *texture;
		OPI_Text::TextComponent *text;
		ButtonTheme *theme;

		std::function<void()> callback;
	};
}
#endif