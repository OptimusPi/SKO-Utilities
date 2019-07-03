#ifndef __OPI_GUI_THEMELOADER_
#define __OPI_GUI_THEMELOADER_

#include <string>
#include <map>

#include "ElementTheme.h"

namespace OPI_Gui
{
	/// Singleton
	class ThemeLoader
	{
	public:
		static OPI_Gui::ElementTheme *GetTheme(std::string theme);
	private:
		static ThemeLoader* getInstance();
		static ThemeLoader *instance;
		ThemeLoader();
		virtual ~ThemeLoader();
		OPI_Gui::ElementTheme *getTheme(std::string theme);
		void loadTheme(std::string theme);
		std::map<std::string, OPI_Gui::ElementTheme*> themes;
	};
}

#endif