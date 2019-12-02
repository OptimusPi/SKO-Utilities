#include "TextComponent.h"

OPI_Text::TextComponent::TextComponent()
{
	content = "";
    R = 255;
    G = 255;
    B = 255;        
}

OPI_Text::TextComponent::TextComponent(std::string content, OPI_Text::Font* font,  int fontPoint, bool wrapped)
{
	// TODO maybe a better way to handle this?
	if (content.length() == 0)
		content = " ";

	// TODO move RGB to parameters with default value
	R = 255;
	G = 255;
	B = 255;
	setSize(fontPoint);
	setText(content, font, wrapped);
}

size_t OPI_Text::TextComponent::length()
{
	// Return length of content string
	return this->content.length();
}

void OPI_Text::TextComponent::renderImage(std::string content, OPI_Text::Font* font, bool wrapped)
{
	if (!font)
	{
		throw new std::logic_error("OPI_Text::renderImage() called with null font!");
	}

	// TODO - implement text wrap
	SDL_Surface *surface = TTF_RenderUTF8_Blended(font->size(this->fontPoint), content.c_str(), this->color);
	this->contentRender.setImage(surface);
	SDL_FreeSurface(surface);
}

void OPI_Text::TextComponent::setText(std::string content, OPI_Text::Font* font, bool wrapped)
{
	this->content = content;

	if (font != NULL)
	{
		this->font = font;
	}

	renderImage(content, this->font, wrapped);
}

void OPI_Text::TextComponent::setSize(int fontPoint)
{
	this->fontPoint = fontPoint;
}
