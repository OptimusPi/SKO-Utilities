#include "TextLabel.h"

OPI_Gui::TextLabel::TextLabel(OPI_Text::TextComponent * text)
{
	this->text = text;
	this->x = text->x;
	this->y = text->y;
	this->setTexture(text->contentRender);
	this->width = this->getTexture()->width;
	this->height = this->getTexture()->height;
}

OPI_Gui::TextLabel::TextLabel(int x, int y, OPI_Text::TextComponent * text)
{
	this->text = text;
	this->x = x;
	this->y = y;
	this->setTexture(text->contentRender);
	this->width = this->getTexture()->width;
	this->height = this->getTexture()->height;
}

void OPI_Gui::TextLabel::setText(OPI_Text::TextComponent *text)
{
	this->text = text;
	this->width = this->getTexture()->width;
	this->height = this->getTexture()->height;
}


OPI_Gui::TextLabel::~TextLabel()
{

}

bool OPI_Gui::TextLabel::handleMouseMove(int mouseX, int mouseY)
{
	return false;
}

bool OPI_Gui::TextLabel::handleMousePressLeft(int mouseX, int mouseY)
{
	return false;
}

bool OPI_Gui::TextLabel::handleMousePressRight(int mouseX, int mouseY)
{
	return false;
}

bool OPI_Gui::TextLabel::handleMouseReleaseRight(int mouseX, int mouseY)
{
	return false;
}

bool OPI_Gui::TextLabel::handleMouseReleaseLeft(int mouseX, int mouseY)
{
	return false;
}

