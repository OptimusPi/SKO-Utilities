#include "Tile.h"

SKO_Map::Tile::Tile()
{

}

SKO_Map::Tile::Tile(int x, int y, unsigned char tileId)
{
	this->x = x;
	this->y = y;
	this->tileId = tileId;
}

SKO_Map::Tile::~Tile()
{

}
