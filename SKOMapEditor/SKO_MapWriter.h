#ifndef __SKO_MAP_WRITER_H_
#define __SKO_MAP_WRITER_H_

#define VERSION_MAJOR 1
#define VERSION_MINOR 0

#include "SKO_Map.h"

namespace SKO_Map
{
	class Writer
	{
	public:
		// Pass a SKO_Map and it will save to its file location
		static void saveMap(SKO_Map::Map *map);

		// Save map header and count
		static void saveMetaData(SKO_Map::Map * map, std::ofstream *file);

		// Save dimensions and graphics
		static void saveBackgroundTiles(SKO_Map::Map * map, std::ofstream *file);
		static void saveFringeTiles(SKO_Map::Map * map, std::ofstream *file);
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
