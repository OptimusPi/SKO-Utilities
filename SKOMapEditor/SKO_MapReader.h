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
		Reader(std::string directoryLocation, std::map<std::string, SKO_Map::Tileset*> tilesets);
		virtual ~Reader();
		
		SKO_Map::Map *loadMap(std::string mapFile);
		std::string fileLocation;
		std::map<std::string, SKO_Map::Tileset*> tilesets;
	private:
		// Load dimensions and graphics
		void loadTileLayer(std::string name, std::map<std::string, std::vector<Tile*>> &tileLayer, INIReader mapIni);
		void loadBackgroundTiles(SKO_Map::Map * map, INIReader mapIni);
		void loadBackgroundMaskTiles(SKO_Map::Map* map, INIReader mapIni);
		void loadFringeTiles(SKO_Map::Map * map, INIReader mapIni);
		void loadFringeMaskTiles(SKO_Map::Map* map, INIReader mapIni);
		void loadCollisionRects(SKO_Map::Map * map, INIReader mapIni);

		// Load game objects
		void loadPortals(SKO_Map::Map * map, INIReader mapIni);
		void loadSigns(SKO_Map::Map * map, INIReader mapIni);
		void loadEnemies(SKO_Map::Map * map, INIReader mapIni);
		void loadStalls(SKO_Map::Map * map, INIReader mapIni);
		void loadShops(SKO_Map::Map * map, INIReader mapIni);
		void loadTargets(SKO_Map::Map * map, INIReader mapIni);
		void loadNpcs(SKO_Map::Map * map, INIReader mapIni);

		// Parsing helper
		std::vector<std::string> parseCsv(std::string values);
		std::vector<int> parseCsvInt(std::string values);
	};
}

#endif