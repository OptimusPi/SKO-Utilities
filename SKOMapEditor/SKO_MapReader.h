#ifndef __SKO_MAP_READER_H_
#define __SKO_MAP_READER_H_

#include "INIReader.h"
#include "SKO_Map.h"

#include <string>

namespace SKO_Map
{
	class Reader
	{
	public:
		Reader(std::string directoryLocation);
		virtual ~Reader();
		
		SKO_Map::Map *loadMap(std::string mapFile);
		std::string fileLocation;
	private:
		// Load dimensions and graphics
		void loadBackgroundTiles(SKO_Map::Map * map, INIReader mapIni);
		void loadFringeTiles(SKO_Map::Map * map, INIReader mapIni);
		void loadCollisionRects(SKO_Map::Map * map, INIReader mapIni);

		// Load game objects
		void loadPortals(SKO_Map::Map * map, INIReader mapIni);
		void loadSigns(SKO_Map::Map * map, INIReader mapIni);
		void loadEnemies(SKO_Map::Map * map, INIReader mapIni);
		void loadStalls(SKO_Map::Map * map, INIReader mapIni);
		void loadShops(SKO_Map::Map * map, INIReader mapIni);
		void loadTargets(SKO_Map::Map * map, INIReader mapIni);
		void loadNpcs(SKO_Map::Map * map, INIReader mapIni);
	};
}

#endif