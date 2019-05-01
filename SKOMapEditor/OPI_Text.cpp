#include "OPI_Text.h"

OPI_Text::OPI_Text()
{
    R = 255;
    G = 255;
    B = 255;        
}

// Return true for success
bool OPI_Text::Init()
{
	//Initialize SDL_ttf
	if (TTF_Init() == -1)
	{
		return false;
	}

	//Open the font
	auto font = TTF_OpenFont("font.ttf", 12);

	//If there was an error in loading the font
	if (font == NULL)
	{
		return false;
	}
}
size_t OPI_Text::length()
{
	// Return length of content string
	return this->content.length();
}

void OPI_Text::generateImage(std::string text, std::string font)
{
	SDL_Surface *surface = TTF_RenderUTF8_Blended(font, "(1000, 42069)", color);
}


void OPI_Text::SetText(std::string content)
{
	this->content = content;
	this->contentRender = generateImage();
}
