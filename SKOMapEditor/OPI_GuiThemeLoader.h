#ifndef __OPI_GUI_THEMELOADER_
#define __OPI_GUI_THEMELOADER_

#include <string>
#include <map>

#include "OPI_GuiElementTheme.h"
#include "OPI_GuiElementThemeType.h"

namespace OPI_Gui
{
	/// Singleton
	class ThemeLoader
	{
	public:
		static OPI_Gui::ElementTheme *GetTheme(OPI_Gui::ElementThemeType type, std::string theme);
		OPI_Gui::ElementThemeType t;
	private:
		static ThemeLoader* getInstance();
		static ThemeLoader *instance;
		ThemeLoader();
		virtual ~ThemeLoader();

		// Grab theme from cache if it exists, 
		// else load it from disk.
		OPI_Gui::ElementTheme *getTheme(OPI_Gui::ElementThemeType type, std::string theme);

		// Load themes from disk and add them to cache
		void loadTheme_GridRect(std::string theme);
		void loadTheme_Image(std::string theme);
		void loadTheme_Button(std::string theme);

		// Get a unique key to look up in the dictionary
		std::string generateKey(OPI_Gui::ElementThemeType type, std::string theme);

		// Dictionary to cache all the themes
		std::map<std::string, OPI_Gui::ElementTheme*> themes;
	};
}

#endif