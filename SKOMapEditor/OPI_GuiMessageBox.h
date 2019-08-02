#ifndef	__OPI_GUIMESSAGEBOX_
#define __OPI_GUIMESSAGEBOX_

#include "OPI_GuiElement.h"
#include "OPI_GuiMessageBoxType.h"
#include "OPI_GuiElementThemeType.h"
#include "OPI_GuiThemeLoader.h"
#include "OPI_Image.h"
#include "OPI_Text.h"
#include "OPI_GuiTextLabel.h"
#include "OPI_GuiButton.h"
#include "OPI_GuiPanel.h"

namespace OPI_Gui
{
	class MessageBox : public Panel
	{
	public:
		MessageBox(ElementThemeType themeType, std::string theme, std::string message, TTF_Font* font, bool wordWrap = false);
		MessageBox(ElementThemeType themeType, std::string theme, std::string message, MessageBoxType messageBoxType, TTF_Font* font, bool wordWrap = false);
		virtual ~MessageBox();
		void setText(std::string message);
		
		OPI_Image *texture;
		ElementTheme *theme;
		MessageBoxType type = MessageBoxType::Okay;

		OPI_Text *message;
		TTF_Font *font;
		int width;
		int height;
		bool wordWrap = false;
		const int DefaultPadding = 32;
	private:
		void addButtons();
		void addText();
	};
}

#endif

