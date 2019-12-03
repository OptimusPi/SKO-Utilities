#include "SKO_MapWriter.h"

void SKO_Map::Writer::saveMap(SKO_Map::Map * map)
{
	// Open the output, overwriting the entire file.
	std::ofstream *file = new std::ofstream(map->filePath);

	// Save map header and count
	saveMetaData(map, file);

	// Save dimensions and graphics
	*file << std::endl;
	*file << "# Map Dimensions: Background Tiles, Fringe Tiles, and Collision Rectangles" << std::endl;
	*file << std::endl;
	saveBackgroundTiles(map, file);
	saveFringeTiles(map, file);
	saveCollisionRects(map, file);

	// Save game objects
	*file << std::endl;
	*file << "# Game Content: Portals, Signs, Enemies, Stalls, Shops, Targets, Npcs" << std::endl;
	*file << std::endl;
	savePortals(map, file);
	saveSigns(map, file);
	saveEnemies(map, file);
	saveStalls(map, file);
	saveShops(map, file);
	saveTargets(map, file);
	saveNpcs(map, file);
}

void SKO_Map::Writer::saveMetaData(SKO_Map::Map * map, std::ofstream * file)
{
	// Map Version
	*file << "# Stick Knights Online Map File" << std::endl;
	*file << "# This is an auto-generated file" << std::endl;
	*file << "# Any added comments will be removed if this file is opened and saved in the map editor." << std::endl;
	*file << std::endl;
	*file << "[version]" << std::endl;// TODO READER CHANGE
	*file << "version = " << VERSION_MAJOR << "." << VERSION_MINOR << std::endl;// TODO READER CHANGE
	*file << std::endl;

	// Count of all the types
	*file << "[count]" << std::endl;
	*file << "background_tiles = " << map->backgroundTiles.size() << std::endl;
	*file << "fringe_tiles = " << map->fringeTiles.size() << std::endl;
	*file << "collision_rects = " << map->collisionRects.size() << std::endl;
	*file << "portals = " << map->portals.size() << std::endl;
	*file << "signs = " << map->signs.size() << std::endl;
	*file << "enemies = " << map->enemies.size() << std::endl;
	*file << "stalls = " << map->stalls.size() << std::endl;
	*file << "shops = " << map->shops.size() << std::endl;
	*file << "targets = " << map->targets.size() << std::endl;
	*file << "npcs = " << map->npcs.size() << std::endl;
}

void SKO_Map::Writer::saveBackgroundTiles(SKO_Map::Map * map, std::ofstream * file)
{
	for (int i = 0; i < map->backgroundTiles.size(); i++)
	{
		*file << "[background_tile" << i << "]" << std::endl; // TODO READER CHANGE
		*file << "x"  << map->backgroundTiles[i]->x << std::endl;// TODO READER CHANGE
		*file << "y"  << map->backgroundTiles[i]->y << std::endl;// TODO READER CHANGE
		*file << "tile_id" << (int)map->backgroundTiles[i]->tileId << std::endl;// TODO READER CHANGE
		*file << std::endl;
	}
	*file << std::endl;
}

void SKO_Map::Writer::saveFringeTiles(SKO_Map::Map * map, std::ofstream * file)
{
	for (int i = 0; i < map->fringeTiles.size(); i++)
	{
		*file << "[fringe_tile" << i << "]" << std::endl;// TODO READER CHANGE
		*file << "x" << map->fringeTiles[i]->x << std::endl;// TODO READER CHANGE
		*file << "y" << map->fringeTiles[i]->y << std::endl;// TODO READER CHANGE
		*file << "tile_id" << (int)map->fringeTiles[i]->tileId << std::endl;// TODO READER CHANGE
		*file << std::endl;
	}
	*file << std::endl;
}

void SKO_Map::Writer::saveCollisionRects(SKO_Map::Map * map, std::ofstream * file)
{
	for (int i = 0; i < map->collisionRects.size(); i++) 
	{
		*file << "[collision_rects" << i << "]" << std::endl; // TODO READER CHANGE
		*file << "x" << map->collisionRects[i].x << std::endl;// TODO READER CHANGE
		*file << "y" << map->collisionRects[i].y << std::endl;// TODO READER CHANGE
		*file << "w" << map->collisionRects[i].w << std::endl;// TODO READER CHANGE
		*file << "h" << map->collisionRects[i].h << std::endl;// TODO READER CHANGE
		*file << std::endl;
	}
	*file << std::endl;
}

void SKO_Map::Writer::savePortals(SKO_Map::Map * map, std::ofstream * file)
{
	for (int i = 0; i < map->portals.size(); i++)
	{
		*file << "[portal" << i << "]" << std::endl;
		*file << "x" << map->portals[i]->x << std::endl;
		*file << "y" << map->portals[i]->y << std::endl;
		*file << "w" << map->portals[i]->w << std::endl;
		*file << "h" << map->portals[i]->h << std::endl;
		*file << "map_id" << map->portals[i]->mapId << std::endl; // TODO READER CHANGE
		*file << "spawn_x" << map->portals[i]->spawn_x << std::endl;
		*file << "spawn_y" << map->portals[i]->spawn_y << std::endl; 
		*file << "level_required" << map->portals[i]->level_required << std::endl;
		*file << std::endl;
	}
	*file << std::endl;
}

void SKO_Map::Writer::saveSigns(SKO_Map::Map * map, std::ofstream * file)
{
	for (int i = 0; i < map->signs.size(); i++)
	{
		*file << "[sign" << i << "]" << std::endl;
		*file << "x" << map->signs[i].x << std::endl;
		*file << "y" << map->signs[i].y << std::endl;
		*file << std::endl;
	}
	*file << std::endl;
}

void SKO_Map::Writer::saveEnemies(SKO_Map::Map * map, std::ofstream * file)
{
	for (int i = 0; i < map->YYYYYYYYYYYYYYYYYYY.size(); i++)
	{
		*file << "[YYYYYYYYYYYY" << i << "]" << std::endl;
		*file << "x" << map->YYYYYYYYYYYYY[i].x << std::endl;
		*file << "y" << map->YYYYYYYYYYYYY[i].y << std::endl;
		*file << std::endl;
	}
	*file << std::endl;
}

void SKO_Map::Writer::saveStalls(SKO_Map::Map * map, std::ofstream * file)
{
	for (int i = 0; i < map->YYYYYYYYYYYYYYYYYYY.size(); i++)
	{
		*file << "[YYYYYYYYYYYY" << i << "]" << std::endl;
		*file << "x" << map->YYYYYYYYYYYYY[i].x << std::endl;
		*file << "y" << map->YYYYYYYYYYYYY[i].y << std::endl;
		*file << std::endl;
	}
	*file << std::endl;
}

void SKO_Map::Writer::saveShops(SKO_Map::Map * map, std::ofstream * file)
{
	for (int i = 0; i < map->YYYYYYYYYYYYYYYYYYY.size(); i++)
	{
		*file << "[YYYYYYYYYYYY" << i << "]" << std::endl;
		*file << "x" << map->YYYYYYYYYYYYY[i].x << std::endl;
		*file << "y" << map->YYYYYYYYYYYYY[i].y << std::endl;
		*file << std::endl;
	}
	*file << std::endl;
}

void SKO_Map::Writer::saveTargets(SKO_Map::Map * map, std::ofstream * file)
{
	for (int i = 0; i < map->YYYYYYYYYYYYYYYYYYY.size(); i++)
	{
		*file << "[YYYYYYYYYYYY" << i << "]" << std::endl;
		*file << "x" << map->YYYYYYYYYYYYY[i].x << std::endl;
		*file << "y" << map->YYYYYYYYYYYYY[i].y << std::endl;
		*file << std::endl;
	}
	*file << std::endl;
}

void SKO_Map::Writer::saveNpcs(SKO_Map::Map * map, std::ofstream * file)
{
	for (int i = 0; i < map->YYYYYYYYYYYYYYYYYYY.size(); i++)
	{
		*file << "[YYYYYYYYYYYY" << i << "]" << std::endl;
		*file << "x" << map->YYYYYYYYYYYYY[i].x << std::endl;
		*file << "y" << map->YYYYYYYYYYYYY[i].y << std::endl;
		*file << std::endl;
	}
	*file << std::endl;
}
