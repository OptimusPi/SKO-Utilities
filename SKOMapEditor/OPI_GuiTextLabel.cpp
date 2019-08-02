#include "OPI_GuiTextLabel.h"

OPI_Gui::TextLabel::TextLabel(int x, int y, OPI_Text * text)
{
	this->text = text;
	this->x = x;
	this->y = y;
	this->setTexture(&text->contentRender);
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
