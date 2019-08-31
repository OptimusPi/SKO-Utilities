#include "OPI_Text.h"

OPI_Text::OPI_Text()
{
	content = "";
    R = 255;
    G = 255;
    B = 255;        
}

OPI_Text::OPI_Text(std::string content, OPI_Font* font,  int fontPoint, bool wrapped)
{
	//TODO move RGB to parameters with default value
	R = 255;
	G = 255;
	B = 255;
	setSize(fontPoint);
	setText(content, font, wrapped);
}

size_t OPI_Text::length()
{
	// Return length of content string
	return this->content.length();
}

void OPI_Text::renderImage(std::string content, OPI_Font* font, bool wrapped)
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

void OPI_Text::setText(std::string content, OPI_Font* font, bool wrapped)
{
	this->content = content;

	if (font != NULL)
	{
		this->font = font;
	}

	renderImage(content, this->font, wrapped);
}

void OPI_Text::setSize(int fontPoint)
{
	this->fontPoint = fontPoint;
}
