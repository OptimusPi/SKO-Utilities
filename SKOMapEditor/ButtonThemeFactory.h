#ifndef __OPI_GUI_BUTTONTHEMEFACTORY_H_
#define __OPI_GUI_BUTTONTHEMEFACTORY_H_

#include <string>
#include <map>

#include "ElementThemeType.h"
#include "ButtonTheme.h"

namespace OPI_Gui
{
	class ButtonThemeFactory
	{
	public:
		static OPI_Gui::ButtonTheme *GetTheme(OPI_Gui::ElementThemeType type, std::string theme);
		OPI_Gui::ElementThemeType t;
	private:
		static ButtonThemeFactory* getInstance();
		static ButtonThemeFactory *instance;
		ButtonThemeFactory();
		virtual ~ButtonThemeFactory();

		// Grab theme from cache if it exists, 
		// else load it from disk.
		OPI_Gui::ButtonTheme *getTheme(OPI_Gui::ElementThemeType type, std::string theme);

		// Load themes from disk and add them to cache
		void loadTheme_Button(std::string theme);
		void loadTheme_ButtonIcon(std::string themeImage);

		// Get a unique key to look up in the dictionary
		std::string generateKey(OPI_Gui::ElementThemeType type, std::string theme);

		// Dictionary to cache all the themes
		std::map<std::string, OPI_Gui::ButtonTheme*> themes;
	};
}
#endif