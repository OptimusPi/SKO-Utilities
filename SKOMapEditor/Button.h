#ifndef __OPI_BUTTON_
#define __OPI_BUTTON_

#include <functional>

#include "Element.h"
#include "OPI_Image.h"
#include "OPI_Text.h"

#include <string>

namespace OPI_Gui
{
	class Button : public Element
	{
	public:
		Button(std::string theme, int x = 0, int y = 0);
		virtual ~Button();
		void addCallback(std::function<void(void)> callback);
	private:
		// Button position and dimensions inside its OPI_Panel
		int width;
		int height;
		OPI_Image *textureEnabled;
		OPI_Image *textureDisabled;
		OPI_Image *texturePressed;
		OPI_Image *textureSelected;
		OPI_Text *text;
		std::function<void(void)> callback;
		void loadTheme(std::string theme);
	};
}
#endif