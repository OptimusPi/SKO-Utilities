#include "OPI_Text.h"

OPI_Text::OPI_Text()
{
    R = 255;
    G = 255;
    B = 255;        
}

size_t OPI_Text::length()
{
	// Return length of content string
	return this->content.length();
}

void OPI_Text::SetText(std::string content)
{
	this->content = content;
}
