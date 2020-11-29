#ifndef __SKO_MAP_WRITER_H_
#define __SKO_MAP_WRITER_H_

#define VERSION_MAJOR 1
#define VERSION_MINOR 2

#include "SKO_Map.h"

namespace SKO_Map
{
	class Writer
	{
	public:
		// Pass a SKO_Map and it will save to its file location
		static void saveMap(SKO_Map::Map *map, std::map<std::string, SKO_Map::Tileset*> tilesets);

		// Save map header and count
		static void saveMetaData(SKO_Map::Map * map, std::ofstream *file);
		static int countTiles(std::map<std::string, std::vector<SKO_Map::Tile*>> tiles);

		// Save dimensions and graphics
		static void saveBackgroundTiles(SKO_Map::Map * map, std::map<std::string, SKO_Map::Tileset*> tilesets, std::ofstream *file);
		static void saveFringeTiles(SKO_Map::Map * map, std::map<std::string, SKO_Map::Tileset*> tilesets, std::ofstream *file);
		static void saveCollisionRects(SKO_Map::Map * map, std::ofstream *file);

		// Save game objects
		static void savePortals(SKO_Map::Map * map, std::ofstream *file);
		static void saveSigns(SKO_Map::Map * map, std::ofstream *file);
		static void saveEnemies(SKO_Map::Map * map, std::ofstream *file);
		static void saveStalls(SKO_Map::Map * map, std::ofstream *file);
		static void saveShops(SKO_Map::Map * map, std::ofstream *file);
		static void saveTargets(SKO_Map::Map * map, std::ofstream *file);
		static void saveNpcs(SKO_Map::Map * map, std::ofstream *file);
	};
}


#endif
