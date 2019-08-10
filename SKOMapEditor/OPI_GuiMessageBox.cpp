#include "OPI_GuiMessageBox.h"

OPI_Gui::MessageBox::~MessageBox()
{

}

bool OPI_Gui::MessageBox::isInteracting()
{
	return true;
}

OPI_Gui::MessageBox::MessageBox(std::string message, TTF_Font* font, bool wordWrap, OPI_Gui::ElementThemeType themeType, std::string theme)
	: Panel(themeType, theme, 0, 0)
{
	this->theme = OPI_Gui::ThemeLoader::GetTheme(themeType, theme);
	this->message = new OPI_Text(message, font, wordWrap);
	this->wordWrap = wordWrap;
	this->addText();
	this->addButtons();
	this->theme->render(this);
	this->x = OPI_Gui::GuiManager::getInstance()->getScreenWidth() / 2 - this->width / 2;
	this->x = OPI_Gui::GuiManager::getInstance()->getScreenHeight() / 2 - this->height / 2;
	this->isVisible = true;
}

OPI_Gui::MessageBox::MessageBox(std::string message, OPI_Gui::MessageBoxType messageBoxType, TTF_Font* font, bool wordWrap, OPI_Gui::ElementThemeType themeType, std::string theme)
	: Panel(themeType, theme, 0, 0)
{
	this->theme = OPI_Gui::ThemeLoader::GetTheme(themeType, theme);
	this->message = new OPI_Text(message, font, wordWrap);
	this->wordWrap = wordWrap;
	this->theme->render(this);
	this->addText();
	this->addButtons();
	this->isVisible = true;
}

void OPI_Gui::MessageBox::setText(std::string message)
{
	this->message->setText(message, this->font, this->wordWrap);
	this->theme->render(this);
}

void OPI_Gui::MessageBox::addText()
{
	OPI_Gui::TextLabel *textLabel = new OPI_Gui::TextLabel(this->width/2, this->height/2, this->message);
	textLabel->isVisible = true;
	addElement(textLabel);
	setWidth(textLabel->getTexture()->width + this->DefaultPadding * 2);
	setHeight(textLabel->getTexture()->height + this->DefaultPadding * 2);
}

void OPI_Gui::MessageBox::addButtons()
{
	int buttonWidth = ThemeLoader::GetTheme(OPI_Gui::ElementThemeType::Button, "default")->getMaximumWidth();
	int buttonHeight = ThemeLoader::GetTheme(OPI_Gui::ElementThemeType::Button, "default")->getMaximumHeight();
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

	switch (this->type)
	{
	case OPI_Gui::MessageBoxType::Okay:
		okButtonX = (this->width / 2) - (buttonWidth / 2);
		okButtonY = this->height - this->DefaultPadding/2 - buttonHeight;
		okayButton = new OPI_Gui::Button("default", okButtonX, okButtonY, "Okay");
		okayButton->isVisible = true;
		addElement(okayButton);
		break;
	case OPI_Gui::MessageBoxType::OkayCancel:
		break;
	case OPI_Gui::MessageBoxType::YesNo:
		break;
	}
}
