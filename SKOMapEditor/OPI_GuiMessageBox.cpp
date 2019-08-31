#include "OPI_GuiMessageBox.h"

OPI_Gui::MessageBox::~MessageBox()
{

}

bool OPI_Gui::MessageBox::isInteracting()
{
	return true;
}

OPI_Gui::MessageBox::MessageBox(std::string message, OPI_Font* font, bool wordWrap, OPI_Gui::ElementThemeType themeType, std::string theme)
	: Panel(themeType, theme, 0, 0)
{
	this->theme = OPI_Gui::ThemeLoader::GetTheme(themeType, theme);

	this->buttonWidth = ThemeLoader::GetTheme(OPI_Gui::ElementThemeType::Button, "default")->getMaximumWidth();
	this->buttonHeight = ThemeLoader::GetTheme(OPI_Gui::ElementThemeType::Button, "default")->getMaximumHeight();
	this->buttonRowHeight = this->buttonHeight + DefaultPadding;

	this->message = new OPI_Text(message, font, wordWrap);
	this->wordWrap = wordWrap;
	this->addText();
	this->addButtons();
	this->theme->render(this);
	this->x = OPI_Gui::GuiManager::getScreenWidth() / 2 - this->width / 2;
	this->y = OPI_Gui::GuiManager::getScreenHeight() / 2 - this->height / 2;
	this->isVisible = true;
	this->isMovable = true;
}


//TODO don't duplicate constructor code
OPI_Gui::MessageBox::MessageBox(std::string message, OPI_Gui::MessageBoxType messageBoxType, OPI_Font* font, bool wordWrap, OPI_Gui::ElementThemeType themeType, std::string theme)
	: Panel(themeType, theme, 0, 0)
{
	this->theme = OPI_Gui::ThemeLoader::GetTheme(themeType, theme);

	this->buttonWidth = ThemeLoader::GetTheme(OPI_Gui::ElementThemeType::Button, "default")->getMaximumWidth();
	this->buttonHeight = ThemeLoader::GetTheme(OPI_Gui::ElementThemeType::Button, "default")->getMaximumHeight();
	this->buttonRowHeight = this->buttonHeight + DefaultPadding;

	this->message = new OPI_Text(message, font, wordWrap);
	this->wordWrap = wordWrap;	
	this->addText();
	this->addButtons();
	this->theme->render(this);
	this->x = OPI_Gui::GuiManager::getScreenWidth() / 2 - this->width / 2;
	this->y = OPI_Gui::GuiManager::getScreenHeight() / 2 - this->height / 2;
	this->isVisible = true;
}

void OPI_Gui::MessageBox::setText(std::string message)
{
	this->message->setText(message, this->font, this->wordWrap);
	this->theme->render(this);
}

void OPI_Gui::MessageBox::addText()
{
	// Create TextLabel to put on this Panel.
	OPI_Gui::TextLabel *textLabel = new OPI_Gui::TextLabel(0, 0, this->message);
	textLabel->isVisible = true;
	addElement(textLabel);

	// Resize the Panel to be able to fit the Text Label.
	setWidth(textLabel->getTexture()->width + this->DefaultPadding * 2);
	setHeight(textLabel->getTexture()->height + this->DefaultPadding * 2 + this->buttonRowHeight);

	// Now, re-center the Text Label after Re-sizing the Panel.
	// Center the text message horizontally on the Panel.
	textLabel->x = (this->width / 2) - (textLabel->width / 2);

	// Center the text vertically between the top of the buttons and the top of the Panel.
	textLabel->y = ((this->height - this->buttonRowHeight) - (textLabel->height/2)) / 2;
}

void OPI_Gui::MessageBox::addButtons()
{
	int okButtonX = 0;
	int okButtonY = 0;
	int cancelButtonX = 0;
	int cancelButtonY = 0;
	int yesButtonX = 0;
	int yesButtonY = 0;
	int noButtonX = 0;
	int noButtonY = 0;
	OPI_Gui::Button *okayButton = nullptr;
	OPI_Gui::Button *cancelButton = nullptr;
	OPI_Gui::Button *yesButton = nullptr;
	OPI_Gui::Button *noButton = nullptr;
	std::function<void()> okayCallback;
	std::function<void()> cancelCallback;
	std::function<void()> yesCallback;
	std::function<void()> noCallback;
	OPI_Text* okText = new OPI_Text("Okay", this->font, 14);

	switch (this->type)
	{
	case OPI_Gui::MessageBoxType::Okay:
		// Position the Okay button in the center of the button row.
		okButtonX = (this->width / 2) - (buttonWidth / 2);
		okButtonY = this->height - this->DefaultPadding - buttonHeight;

		// Create Okay button with default theme & font.
		okayButton = new OPI_Gui::Button("default", okButtonX, okButtonY, okText);
		okayButton->isVisible = true;

		// Set click handler of Okay to close this Message Box.
		okayCallback = [this]() {
			this->destroy();
		};
		okayButton->addCallback(okayCallback);

		// Add Okay button
		addElement(okayButton);
		break;
	case OPI_Gui::MessageBoxType::OkayCancel:
		break;
	case OPI_Gui::MessageBoxType::YesNo:
		break;
	}
}
