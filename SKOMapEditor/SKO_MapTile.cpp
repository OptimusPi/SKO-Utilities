#include "SKO_MapTile.h"

SKO_Map::Tile::Tile()
{

}

SKO_Map::Tile::Tile(int x, int y, std::string tileset_key, unsigned int tileset_row, unsigned int tileset_column)
{
	this->x = x;
	this->y = y;
	this->tileset_key = tileset_key;
	this->tileset_row = tileset_row;
	this->tileset_column = tileset_column;
}


SKO_Map::Tile::Tile(int x, int y, SKO_Map::Tileset *tileset, unsigned int tileset_row, unsigned int tileset_column)
{
	this->x = x;
	this->y = y;
	this->tileset_key = tileset->key;
	this->tileset_row = tileset_row;
	this->tileset_column = tileset_column;
	this->image = tileset->getTileImage(tileset_row, tileset_column);
}

SKO_Map::Tile::~Tile()
{

}

bool SKO_Map::Tile::same(Tile* tileA, Tile* tileB)
{
	if (tileA->tileset_key != tileB->tileset_key)
	{
		return false;
	}

	if (tileA->tileset_row != tileB->tileset_row)
	{
		return false;
	}

	if (tileA->tileset_column != tileB->tileset_column)
	{
		return false;
	}

	if (tileA->x != tileB->x)
	{
		return false;
	}

	if (tileA->y != tileB->y)
	{
		return false;
	}

	return true;
}
