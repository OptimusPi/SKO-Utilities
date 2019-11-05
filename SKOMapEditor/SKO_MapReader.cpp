#include "SKO_MapReader.h"
#include <fstream>
#include <exception>
#include <sstream>

// SKO Models
#include "SKO_Portal.h" // TODO import models from SKO-Server

SKO_Map::Map * SKO_Map::Reader::loadMap(std::string filePath)
{
	SKO_Map::Map *map = new SKO_Map::Map();
	map->filePath = filePath;

	// Open the file to read
	INIReader mapIni(filePath);
	if (mapIni.ParseError() < 0) {
		std::string error = "Failed to load INI file: " + filePath;
		throw new std::ios_base::failure(error);
	}

	loadPortals(map, mapIni);
}

void SKO_Map::Reader::loadPortals(SKO_Map::Map * map, INIReader mapIni)
{
	//load portals
	int num_portals = mapIni.GetInteger("count", "portals", 0);
	printf("num_portals is %i", num_portals);
	for (int portal = 0; portal < num_portals; portal++)
	{
		std::stringstream ss;
		ss << "portal" << portal;

		//load portal parameters
		Portal[portal] = new SKO_Portal();
		Portal[portal]->x = maIni.GetInteger(ss.str(), "x", 0);
		Portal[portal]->y = maIni.GetInteger(ss.str(), "y", 0);
		Portal[portal]->w = maIni.GetInteger(ss.str(), "w", 0);
		Portal[portal]->h = maIni.GetInteger(ss.str(), "h", 0);
		Portal[portal]->mapId = maIni.GetInteger(ss.str(), "map", 0);
		Portal[portal]->spawn_x = maIni.GetInteger(ss.str(), "spawn_x", 0);
		Portal[portal]->spawn_y = maIni.GetInteger(ss.str(), "spawn_y", 0);
		Portal[portal]->level_required = maIni.GetInteger(ss.str(), "level_required", 0);
	}
}




