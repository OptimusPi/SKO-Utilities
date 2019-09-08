#include "Font.h"

OPI_Text::Font::Font(std::string fontPath)
{
	this->fontPath = fontPath;
}

OPI_Text::Font::Font()
{
}

OPI_Text::Font::~Font()
{
	for (auto keyValue: this->fontSizes) {
		TTF_CloseFont(keyValue.second);
	}
}

TTF_Font* OPI_Text::Font::size(int fontPoint)
{
	// Check dictionary if this size exists, if not, then lazy load
	if (this->fontSizes.count(fontPoint) == 0)
	{
		auto font = loadFont(this->fontPath, fontPoint);
		this->fontSizes.insert({ {fontPoint, font} });
	}

	// Return SDL_ttf object from memory cache
	return this->fontSizes[fontPoint];
}

void OPI_Text::Font::InitializeSDL_ttf()
{
	//Initialize SDL_ttf
	if (TTF_Init() == -1)
	{
		throw new std::logic_error("Could not initialize SDL_ttf");
	}
}

TTF_Font * OPI_Text::Font::loadFont(std::string fontPath, int fontPoint)
{
	// Load font with the given fontPoint
	TTF_Font *font = TTF_OpenFont(fontPath.c_str(), fontPoint);

	if (font == nullptr)
	{
		// Could not load font so bubble up exception
		throw new std::logic_error("Font::loadFont(" + fontPath + ") failed!");
	}

	// Return loaded SDL_ttf font file
	return font;
}
