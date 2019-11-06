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
		static SKO_Map::Map *loadMap(std::string filePath);
	private:
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