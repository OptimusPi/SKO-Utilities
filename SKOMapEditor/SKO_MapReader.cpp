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
	// load portals
	int num_portals = mapIni.GetInteger("count", "portals", 0);
	printf("num_portals is %i", num_portals);

	for (int portal = 0; portal < num_portals; portal++)
	{
		std::stringstream ss;
		ss << "portal" << portal;

		// load portal parameters
		SKO_Portal * portal = new SKO_Portal();
		portal->x = mapIni.GetInteger(ss.str(), "x", 0);
		portal->y = mapIni.GetInteger(ss.str(), "y", 0);
		portal->w = mapIni.GetInteger(ss.str(), "w", 0);
		portal->h = mapIni.GetInteger(ss.str(), "h", 0); 
		portal->mapId = mapIni.GetInteger(ss.str(), "map", 0);
		portal->spawn_x = mapIni.GetInteger(ss.str(), "spawn_x", 0);
		portal->spawn_y = mapIni.GetInteger(ss.str(), "spawn_y", 0);
		portal->level_required = mapIni.GetInteger(ss.str(), "level_required", 0);

		// Add portal to map collection
		map->portals->push_back(portal);
	}
}




