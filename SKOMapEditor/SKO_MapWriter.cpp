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
	*file << "background_tiles =  = \t\t" << map->backgroundTiles.size() << std::endl;
	*file << "fringe_tiles =  = \t\t" << map->fringeTiles.size() << std::endl;
	*file << "collision_rects =  = \t\t" << map->collisionRects.size() << std::endl;
	*file << "portals =  = \t\t" << map->portals.size() << std::endl;
	*file << "signs =  = \t\t" << map->signs.size() << std::endl;
	*file << "enemies =  = \t\t" << map->enemies.size() << std::endl;
	*file << "stalls =  = \t\t" << map->stalls.size() << std::endl;
	*file << "shops =  = \t\t" << map->shops.size() << std::endl;
	*file << "targets =  = \t\t" << map->targets.size() << std::endl;
	*file << "npcs =  = \t\t" << map->npcs.size() << std::endl;
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
		*file << "x = \t\t" << map->fringeTiles[i]->x << std::endl;// TODO READER CHANGE
		*file << "y = \t\t" << map->fringeTiles[i]->y << std::endl;// TODO READER CHANGE
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
		*file << "x = \t\t" << map->collisionRects[i].x << std::endl;// TODO READER CHANGE
		*file << "y = \t\t" << map->collisionRects[i].y << std::endl;// TODO READER CHANGE
		*file << "w = \t\t" << map->collisionRects[i].w << std::endl;// TODO READER CHANGE
		*file << "h = \t\t" << map->collisionRects[i].h << std::endl;// TODO READER CHANGE
		*file << std::endl;
	}
	*file << std::endl;
}

void SKO_Map::Writer::savePortals(SKO_Map::Map * map, std::ofstream * file)
{
	for (int i = 0; i < map->portals.size(); i++)
	{
		*file << "[portal" << i << "]" << std::endl;
		*file << "x = \t\t" << map->portals[i]->x << std::endl;
		*file << "y = \t\t" << map->portals[i]->y << std::endl;
		*file << "w = \t\t" << map->portals[i]->w << std::endl;
		*file << "h = \t\t" << map->portals[i]->h << std::endl;
		*file << "map_id = \t\t" << map->portals[i]->mapId << std::endl; // TODO READER CHANGE
		*file << "spawn_x = \t\t" << map->portals[i]->spawn_x << std::endl;
		*file << "spawn_y = \t\t" << map->portals[i]->spawn_y << std::endl; 
		*file << "level_required = \t\t" << map->portals[i]->level_required << std::endl;
		*file << std::endl;
	}
	*file << std::endl;
}

void SKO_Map::Writer::saveSigns(SKO_Map::Map * map, std::ofstream * file)
{
	for (int i = 0; i < map->signs.size(); i++)
	{
		*file << "[sign" << i << "]" << std::endl;
		*file << "x = \t\t" << map->signs[i]->x << std::endl;
		*file << "y = \t\t" << map->signs[i]->y << std::endl;
		*file << "w = \t\t" << map->signs[i]->w << std::endl;
		*file << "h = \t\t" << map->signs[i]->h << std::endl;
		for (int line = 0; line < map->signs[i]->lines.size(); line++)
		{
			*file << "line" << line << " = \"" << map->signs[i]->lines[line]->content << std::endl;
		}
		*file << std::endl;
	}
	*file << std::endl;
}

void SKO_Map::Writer::saveEnemies(SKO_Map::Map * map, std::ofstream * file)
{
	// Save enemy spawn points. 
	// Actual enemy type definitions are ina  separate file 
	// TODO What file? right now it's DAT/enemySprites.ini
	for (int i = 0; i < map->enemies.size(); i++)
	{
		*file << "[enemy" << i << "]" << std::endl;
		*file << "type = \t\t" << map->enemies[i]->type << std::endl;
		*file << "spawn_x = \t\t" << map->enemies[i]->spawn_x << std::endl;
		*file << "spawn_y = \t\t" << map->enemies[i]->spawn_y << std::endl;
		*file << std::endl;
	}
	*file << std::endl;
}

void SKO_Map::Writer::saveStalls(SKO_Map::Map * map, std::ofstream * file)
{
	for (int i = 0; i < map->stalls.size(); i++)
	{
		*file << "[stall" << i << "]" << std::endl;
		*file << "type = \t\t" << map->stalls[i]->type << std::endl;
		*file << "x = \t\t" << map->stalls[i]->x << std::endl;
		*file << "y = \t\t" << map->stalls[i]->y << std::endl;
		*file << "w = \t\t" << map->stalls[i]->w << std::endl;
		*file << "h = \t\t" << map->stalls[i]->h << std::endl;
		*file << std::endl;
	}
	*file << std::endl;
}

void SKO_Map::Writer::saveShops(SKO_Map::Map * map, std::ofstream * file)
{
	for (int i = 0; i < map->YYYYYYYYYYYYYYYYYYY.size(); i++)
	{
		*file << "[YYYYYYYYYYYY" << i << "]" << std::endl;
		*file << "x = \t\t" << map->YYYYYYYYYYYYY[i].x << std::endl;
		*file << "y = \t\t" << map->YYYYYYYYYYYYY[i].y << std::endl;
		*file << std::endl;
	}
	*file << std::endl;
}

void SKO_Map::Writer::saveTargets(SKO_Map::Map * map, std::ofstream * file)
{
	for (int i = 0; i < map->YYYYYYYYYYYYYYYYYYY.size(); i++)
	{
		*file << "[YYYYYYYYYYYY" << i << "]" << std::endl;
		*file << "x = \t\t" << map->YYYYYYYYYYYYY[i].x << std::endl;
		*file << "y = \t\t" << map->YYYYYYYYYYYYY[i].y << std::endl;
		*file << std::endl;
	}
	*file << std::endl;
}

void SKO_Map::Writer::saveNpcs(SKO_Map::Map * map, std::ofstream * file)
{
	for (int i = 0; i < map->YYYYYYYYYYYYYYYYYYY.size(); i++)
	{
		*file << "[YYYYYYYYYYYY" << i << "]" << std::endl;
		*file << "x = \t\t" << map->YYYYYYYYYYYYY[i].x << std::endl;
		*file << "y = \t\t" << map->YYYYYYYYYYYYY[i].y << std::endl;
		*file << std::endl;
	}
	*file << std::endl;
}
