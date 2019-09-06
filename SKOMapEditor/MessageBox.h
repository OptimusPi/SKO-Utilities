#ifndef	__OPI_GUIMESSAGEBOX_
#define __OPI_GUIMESSAGEBOX_

#include "Element.h"
#include "MessageBoxType.h"
#include "ElementThemeType.h"
#include "ElementThemeFactory.h"
#include "OPI_Image.h"
#include "OPI_Text.h"
#include "TextLabel.h"
#include "Button.h"
#include "Panel.h"

namespace OPI_Gui
{
	class MessageBox : public Panel
	{
	public:
		MessageBox(std::string message, OPI_Font* font, 
			bool wordWrap = false, ElementThemeType themeType = ElementThemeType::GridRect, std::string theme = "default");
		MessageBox(std::string message, MessageBoxType messageBoxType, OPI_Font* font, 
			bool wordWrap = false, ElementThemeType themeType = ElementThemeType::GridRect, std::string theme = "default");
		virtual ~MessageBox();
		void setText(std::string message);
		
		// Input handlers
		bool isInteracting();

		ElementTheme *theme;
		MessageBoxType type = MessageBoxType::Okay;

		OPI_Text *message;
		OPI_Font *font;
		bool wordWrap = false;
		const int DefaultPadding = 24;
		int buttonRowHeight = 0;
		int buttonHeight = 0;
		int buttonWidth = 0;

	private:
		void addButtons();
		void addText();
	};
}

#endif

