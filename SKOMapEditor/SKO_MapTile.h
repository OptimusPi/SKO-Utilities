#ifndef __SKOMAPTILE_H_
#define __SKOMAPTILE_H_

#include "OPI_Image.h"
#include "SKO_MapTileset.h"

namespace SKO_Map
{
	class Tile
	{
	public:
		Tile();
		Tile(int x, int y, std::string tileset_key, unsigned int tileset_row, unsigned int tileset_column);
		static bool same(Tile* tileA, Tile* tileB);
		virtual ~Tile();
		int x = 0;
		int y = 0;
		std::string tileset_key;
		unsigned int tileset_row;
		unsigned int tileset_column;
	};
}
#endif