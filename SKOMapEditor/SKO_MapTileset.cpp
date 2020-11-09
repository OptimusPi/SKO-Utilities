#include "SKO_MapTileset.h"
#include <sstream>

SKO_Map::Tileset::Tileset(OPI_Image* image, std::string name, std::string key, unsigned short tile_width, unsigned short tile_height, unsigned char rows, unsigned char columns)
{
	this->key = key;
	this->image = image;
	this->name = name;
	this->tile_width = tile_width;
	this->tile_height = tile_height;
	this->rows = rows;
	this->columns = columns;
	/* Load images into separate image files */
	for (int row = 0; row < rows; row++)
	{
		for (int column = 0; column < columns; column++)
		{
			std::string key = this->getTileKey(row, column);
			SDL_Surface *tileImageSurface = OPI_Image::clipSurface(image->surface, column*tile_width, row*tile_height, tile_width, tile_height);
			OPI_Image* tileImage = new OPI_Image(tileImageSurface);
			this->tileImages.insert({key, tileImage});
		}
	}
}

SKO_Map::Tileset::~Tileset() 
{
	// TODO delete this->image ?

}

OPI_Image* SKO_Map::Tileset::getTileImage(unsigned int row, unsigned int column)
{
	if (row >= this->rows || column >= this->columns)
	{
		throw new std::invalid_argument("This tileset does not have a tile at the specified row and column.");
	}

	std::string key = this->getTileKey(row, column);
	return this->tileImages[key];
}

std::string SKO_Map::Tileset::getTileKey(unsigned int row, unsigned int column)
{
	std::stringstream ss;
	ss << row << "_" << column;
	return ss.str();
}

