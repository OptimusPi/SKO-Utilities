#ifndef __OPI_BUTTON_
#define __OPI_BUTTON_

#include <functional>

#include "Element.h"
#include "OPI_Image.h"
#include "OPI_Text.h"
#include "ThemeLoader.h"
#include "Manager.h"

#include <string>

namespace OPI_Gui
{
	class Button : public Element
	{
	public:
		Button(OPI_Gui::ElementThemeType type, std::string theme, int x, int y);
		virtual ~Button();
		void addCallback(std::function<void(void)> callback);
		bool isPressed;
		bool isSelected;
		bool isEnabled;

		// Input handlers
		bool handleMouseMove(int mouseX, int mouseY);
		bool handleMousePressLeft(int mouseX, int mouseY);
		bool handleMousePressRight(int mouseX, int mouseY);
		bool handleMouseReleaseRight(int mouseX, int mouseY);
		bool handleMouseReleaseLeft(int mouseX, int mouseY);
		bool movableContainsMouse(int mouseX, int mouseY);
		bool resizableContainsMouse(int mouseX, int mouseY);
		bool closableContainsMouse(int mouseX, int mouseY);
	private:
		OPI_Gui::ElementTheme *theme;
		OPI_Image *texture;
		OPI_Text *text;

		std::function<void(void)> callback;
	};
}
#endif