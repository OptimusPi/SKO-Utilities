#include "OPI_Text.h"

OPI_Text::OPI_Text()
{
	content = "";
    R = 255;
    G = 255;
    B = 255;        
}

OPI_Text::OPI_Text(std::string content, TTF_Font* font, bool wrapped)
{
	//TODO move RGB to parameters with default value
	R = 255;
	G = 255;
	B = 255;
	setText(content, font, wrapped);
}

size_t OPI_Text::length()
{
	// Return length of content string
	return this->content.length();
}

void OPI_Text::renderImage(std::string content, TTF_Font* font, bool wrapped)
{
	SDL_Surface *surface = TTF_RenderUTF8_Blended(font, content.c_str(), this->color);
	this->contentRender.setImage(surface);
}

void OPI_Text::setText(std::string content, TTF_Font* font, bool wrapped)
{
	this->content = content;

	if (font != NULL)
	{
		this->currentFont = font;
	}

	renderImage(content, this->currentFont, wrapped);
}