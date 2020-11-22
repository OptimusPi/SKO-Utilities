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
}

SKO_Map::Tileset::~Tileset() 
{
	// TODO delete this->image ?

}

