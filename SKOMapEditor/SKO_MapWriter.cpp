#include "SKO_MapWriter.h"

void SKO_Map::Writer::saveMap(SKO_Map::Map * map, std::map<std::string, SKO_Map::Tileset*> tilesets)
{
	// Open the output, overwriting the entire file.
	std::ofstream* file = new std::ofstream(map->filePath); // TODO - remove this, tempoorarily being used to convert old maps to new format... +".2.ini");

	// Save map header and count
	saveMetaData(map, file);

	// Save dimensions and graphics
	*file << std::endl;
	*file << "# Map Dimensions: Background Tiles, Fringe Tiles, and Collision Rectangles" << std::endl;
	*file << std::endl;
	saveBackgroundTiles(map, tilesets, file);
	saveFringeTiles(map, tilesets, file);
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
	*file << "tiles = " << map->backgroundTiles.size() << std::endl;
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

void SKO_Map::Writer::saveBackgroundTiles(SKO_Map::Map * map, std::map<std::string, SKO_Map::Tileset*> tilesets, std::ofstream * file)
{
	for (auto it = map->backgroundTiles.begin(); it != map->backgroundTiles.end(); it++)
	{
		auto tilesetKey = it->first;
		auto tiles = it->second;

		if (tiles.size() > 0)
		{
			*file << "; " << tilesets[tilesetKey]->name;
			*file << "[background_tiles_" << tilesetKey << "]" << std::endl;
		}

		for (int i = 0; i < tiles.size(); i++)
		{
			*file << "x" << i << " = " << tiles[i]->x << std::endl;
			*file << "y" << i << " = " << tiles[i]->y << std::endl;
			*file << "row" << i << " = " << tiles[i]->tileset_row << std::endl;
			*file << "col" << i << " = " << tiles[i]->tileset_column << std::endl;
		}
		
		*file << std::endl;
	}
	*file << std::endl;
}

void SKO_Map::Writer::saveFringeTiles(SKO_Map::Map * map, std::map<std::string, SKO_Map::Tileset*> tilesets, std::ofstream * file)
{
	for (auto it = map->fringeTiles.begin(); it != map->fringeTiles.end(); it++)
	{
		auto tilesetKey = it->first;
		auto tiles = it->second;

		if (tiles.size() > 0)
		{
			*file << "; " << tilesets[tilesetKey]->name;
			*file << "[fringe_tiles_" << tilesetKey << "]" << std::endl;
		}

		for (int i = 0; i < tiles.size(); i++)
		{
			*file << "x" << i << " = " << tiles[i]->x << std::endl;
			*file << "y" << i << " = " << tiles[i]->y << std::endl;
			*file << "row" << i << " = " << tiles[i]->tileset_row << std::endl;
			*file << "col" << i << " = " << tiles[i]->tileset_column << std::endl;
		}

		*file << std::endl;
	}
	*file << std::endl;
}

void SKO_Map::Writer::saveCollisionRects(SKO_Map::Map * map, std::ofstream * file)
{
	for (int i = 0; i < map->collisionRects.size(); i++) 
	{
		*file << "[collision_rect" << i << "]" << std::endl; // TODO READER CHANGE
		*file << "x = " << map->collisionRects[i].x << std::endl; // TODO READER CHANGE
		*file << "y = " << map->collisionRects[i].y << std::endl; // TODO READER CHANGE
		*file << "w = " << map->collisionRects[i].w << std::endl; // TODO READER CHANGE
		*file << "h = " << map->collisionRects[i].h << std::endl; // TODO READER CHANGE
		*file << std::endl;
	}
	*file << std::endl;
}

void SKO_Map::Writer::savePortals(SKO_Map::Map * map, std::ofstream * file)
{
	for (int i = 0; i < map->portals.size(); i++)
	{
		*file << "[portal" << i << "]" << std::endl;
		*file << "x = " << map->portals[i]->x << std::endl;
		*file << "y = " << map->portals[i]->y << std::endl;
		*file << "w = " << map->portals[i]->w << std::endl;
		*file << "h = " << map->portals[i]->h << std::endl;
		*file << "map_id = " << (int)map->portals[i]->mapId << std::endl; // TODO READER CHANGE - need to use map guid
		*file << "spawn_x = " << map->portals[i]->spawn_x << std::endl;
		*file << "spawn_y = " << map->portals[i]->spawn_y << std::endl; 
		*file << "level_required = " << map->portals[i]->level_required << std::endl;
		*file << std::endl;
	}
	*file << std::endl;
}

void SKO_Map::Writer::saveSigns(SKO_Map::Map * map, std::ofstream * file)
{
	for (int i = 0; i < map->signs.size(); i++)
	{
		*file << "[sign" << i << "]" << std::endl;
		*file << "x = " << map->signs[i]->x << std::endl;
		*file << "y = " << map->signs[i]->y << std::endl;
		*file << "w = " << map->signs[i]->w << std::endl;
		*file << "h = " << map->signs[i]->h << std::endl;
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
	// Actual enemy type definitions are in a separate file 
	// TODO What file? right now it's DAT/enemySprites.ini
	for (int i = 0; i < map->enemies.size(); i++)
	{
		*file << "[enemy" << i << "]" << std::endl;
		*file << "type = " << map->enemies[i]->type << std::endl;
		*file << "spawn_x = " << map->enemies[i]->spawn_x << std::endl;
		*file << "spawn_y = " << map->enemies[i]->spawn_y << std::endl;
		*file << std::endl;
	}
	*file << std::endl;
}

void SKO_Map::Writer::saveStalls(SKO_Map::Map * map, std::ofstream * file)
{
	for (int i = 0; i < map->stalls.size(); i++)
	{
		*file << "[stall" << i << "]" << std::endl;
		*file << "type = " << map->stalls[i]->type << std::endl;
		*file << "x = " << map->stalls[i]->x << std::endl;
		*file << "y = " << map->stalls[i]->y << std::endl;
		*file << "w = " << map->stalls[i]->w << std::endl;
		*file << "h = " << map->stalls[i]->h << std::endl;
		*file << std::endl;
	}
	*file << std::endl;
}

void SKO_Map::Writer::saveShops(SKO_Map::Map * map, std::ofstream * file)
{
	for (int i = 0; i < map->shops.size(); i++) 
	{
		*file << "[shop" << i << "]" << std::endl; // TODO READER CHANGE from 1 to 0

		// title of the shop
		*file << "title = " << map->shops[i]->title << std::endl;

		// Items in the shop
		for (int itemX = 0; itemX < 6; itemX++)
		{
			for (int itemY = 0; itemY < 4; itemY++)
			{
				if (map->shops[i]->item[itemX][itemY][0] > 0)
				{
					// TODO make function to clean these complex writes up! 
					*file << "item"
						<< "_x" << itemX
						<< "_y" << itemY
						<< " = " << map->shops[i]->item[itemX][itemY][0]
						<< std::endl;
					*file << "price"
						<< "_x" << itemX
						<< "_y" << map->shops[i]->item[itemX][itemY][1]
						<< std::endl;
				}
			}
		}
		*file << std::endl;
	}
	*file << std::endl;
}

void SKO_Map::Writer::saveTargets(SKO_Map::Map * map, std::ofstream * file)
{
	for (int i = 0; i < map->targets.size(); i++)
	{
		*file << "[target" << i << "]" << std::endl;
		*file << "x = " << map->targets[i]->x << std::endl;
		*file << "y = " << map->targets[i]->y << std::endl;
		*file << "w = " << map->targets[i]->w << std::endl;
		*file << "h = " << map->targets[i]->h << std::endl;
		*file << "type = " << map->targets[i]->type << std::endl;
		*file << std::endl;
	}
	*file << std::endl;
}

void SKO_Map::Writer::saveNpcs(SKO_Map::Map * map, std::ofstream * file)
{
	for (int i = 0; i < map->npcs.size(); i++) 
	{
		*file << "[npc" << i << "]" << std::endl;

		// Spawn position
		*file << "x = " << map->npcs[i]->x << std::endl;
		*file << "y = " << map->npcs[i]->y << std::endl;

		// Quest Id
		*file << "quest = " << map->npcs[i]->y << std::endl;
		*file << std::endl;

		// Pages of quest dialog
		for (int page = 0; page < map->npcs[i]->pages.size(); page++)
		{
			// Lines per page
			for (int line = 0; line < map->npcs[i]->pages[page]->lines.size(); line++)
			{
				// only print out valid lines
				*file << "page_" << page << "_"
					<< "line_" << line
					<< "= " << map->npcs[i]->pages[page]->lines[line]->content << std::endl;
			}
		}

		// Final Page or Question in the dialog
		*file << "final = " << map->npcs[i]->finalPage << std::endl;
	}
	*file << std::endl;
}
