#include "SKO_MapReader.h"
#include <fstream>
#include <exception>
#include <sstream>
#include "TextComponent.h"
#include "FontManager.h"


SKO_Map::Reader::Reader(std::string fileLocation)
{
	if (fileLocation.find_last_of("/") == fileLocation.length() - 1)
	{
		// IF path ends in a / keep it that way
		this->fileLocation = fileLocation;
	}
	else
	{
		// Else add slash to end of file location
		this->fileLocation = fileLocation + "/";
	}
}
SKO_Map::Reader::~Reader()
{

}

SKO_Map::Map * SKO_Map::Reader::loadMap(std::string fileName)
{
	SKO_Map::Map *map = new SKO_Map::Map();
	// Example:
	//		fileLocation for this class is: "MAP/"
	//		fileName for this function call is: "map0"
	//		filePath will be "MAP/map0.map.ini"
	std::string filePath = this->fileLocation + fileName;

	// Open the file to read
	INIReader mapIni(filePath);
	if (mapIni.ParseError() < 0)
	{
		std::string error = "Failed to load INI file: " + filePath;
		throw new std::ios_base::failure(error);
	}

	map->filePath = filePath;

	loadBackgroundTiles(map, mapIni); // TODO fix with new format, it doesnt match other objects
	loadFringeTiles(map, mapIni); // TODO fix with new format, it doesnt match other objects
	loadCollisionRects(map, mapIni); // TODO fix with new format, it doesnt match other objects

	loadPortals(map, mapIni);
	loadSigns(map, mapIni);
	loadEnemies(map, mapIni); // TODO fix with new format, it doesnt match other objects
	loadStalls(map, mapIni);  
	loadShops(map, mapIni);   
	loadTargets(map, mapIni); 
	loadNpcs(map, mapIni);

	// TODO loadNatureEngine(); // tee hee

	return map;
}


void SKO_Map::Reader::loadBackgroundTiles(SKO_Map::Map * map, INIReader mapIni)
{
	// load background tiles
	int num_background_tiles = mapIni.GetInteger("count", "background_tiles", 0);
	printf("num_background_tiles is %i", num_background_tiles);

	for (int i = 0; i < num_background_tiles; i++)
	{
		std::stringstream sspawn_x;
		sspawn_x << "background_tile_x_" << i;

		std::stringstream sspawn_y;
		sspawn_y << "background_tile_y_" << i;

		std::stringstream ssid;
		ssid << "background_tile_id_" << i;

		int x = mapIni.GetInteger("background_tiles", sspawn_x.str(), 0);
		int y = mapIni.GetInteger("background_tiles", sspawn_y.str(), 0);
		int id = mapIni.GetInteger("background_tiles", ssid.str(), 0);

		// Add background tile to map collection
		SKO_Map::Tile* backgroundTile = convertTile(x, y, id);
		map->backgroundTiles.push_back(backgroundTile);
	}
}

//void SKO_Map::Reader::loadBackgroundTiles(SKO_Map::Map * map, INIReader mapIni)
//{
//	// load background tiles
//	int num_background_tiles = mapIni.GetInteger("count", "background_tiles", 0);
//	printf("num_background_tiles is %i", num_background_tiles);
//
//	for (int i = 0; i < num_background_tiles; i++)
//	{
//		std::stringstream section;
//		section << "background_tile" << i;
//
//
//		int x = mapIni.GetInteger(section.str(), "x", 0);
//		int y = mapIni.GetInteger(section.str(), "y", 0);
//
//		// TODO gotta Ctrl+H the map files... ugh, but wait I can't because of this: background_tile_id_26 = 5
//		//std::string tileset_key = mapIni.Get(section.str(), "tileset_key", "00000000-0000-0000-0000-000000000000");
//		//int tileset_row = mapIni.GetInteger(section.str(), "tileset_row", 0);
//		//int tileset_column = mapIni.GetInteger(section.str(), "tileset_column", 0);
//		
//
//		int id = mapIni.GetInteger(section.str(), ssid.str(), 0);
//
//		// Add fringe tile to map collection
//		SKO_Map::Tile* fringeTile = convertTile(x, y, id);
//		map->fringeTiles.push_back(fringeTile);
//
//		// Add background tile to map collection
//		SKO_Map::Tile *backgroundTile = new SKO_Map::Tile(x, y, tileset_key, tileset_row, tileset_column);
//		map->backgroundTiles.push_back(backgroundTile);
//	}
//}

void SKO_Map::Reader::loadFringeTiles(SKO_Map::Map * map, INIReader mapIni)
{
	// load fringe tiles
	int num_fringe_tiles = mapIni.GetInteger("count", "fringe_tiles", 0);
	printf("num_fringe_tiles is %i", num_fringe_tiles);

	for (int i = 0; i < num_fringe_tiles; i++)
	{
		std::stringstream sspawn_x;
		sspawn_x << "fringe_tile_x_" << i;

		std::stringstream sspawn_y;
		sspawn_y << "fringe_tile_x_" << i;

		std::stringstream ssid;
		ssid << "fringe_tile_id_" << i;

		int x = mapIni.GetInteger("fringe_tiles", sspawn_x.str(),  0);
		int y = mapIni.GetInteger("fringe_tiles", sspawn_y.str(),  0);
		int id = mapIni.GetInteger("fringe_tiles", ssid.str(), 0);

		// Add fringe tile to map collection
		SKO_Map::Tile* fringeTile = convertTile(x, y, id);
		map->fringeTiles.push_back(fringeTile);
	}
}

std::string SKO_Map::Reader::convertKey(int id)
{
	//TODO make these really work
	switch (id)
	{
	case 33:
	case 34:
	case 35:
	case 36:
	case 37:
	case 38:
	case 39:
	case 40:
	case 41:
	case 42:
	case 43:
	case 44:
	case 45:
		return "d9afe501-8273-49f8-ab3e-fb2198bf6826"; // grassland flora

	default:
		return "8a50c312-a146-460f-8120-fbf352c5a95e"; // grassland tiles
	}
}

unsigned int SKO_Map::Reader::convertColumn(int id)
{
	//TODO make these really work
	switch (id)
	{
	case 0:
		return 0;
	case 1:
		return 2;
	case 2:
		return 1;
	case 3:
		return 2;
	case 4:
		return 0;
	case 5:
		return 1;
	case 6:
		return 0;
	case 7:
		return 2;
	case 8:
		return 1;
	case 9:
		return 3;
	case 10:
		return 4;
	case 11:
		return 4;
	case 12:
		return 3;
	case 13:
		return 4;
	case 14:
		return 3;
	case 15:
		return 0;
	case 16:
		return 2;
	case 17:
		return 1;
	case 18:
		return 2;
	case 19:
		return 0;
	case 20:
		return 1;
	case 21:
		return 0;
	case 22:
		return 2;
	case 23:
		return 1;
	case 24:
		return 3;
	case 25:
		return 4;
	case 26:
		return 4;
	case 27:
		return 3;
	case 28:
		return 4;
	case 29:
		return 3;
	case 30:
		return 1;
	case 31:
		return 0;
	case 32:
		return 2;
	case 33:
		return 0;
	case 34:
		return 1;
	case 35:
		return 2;
	case 36:
		return 3;
	case 37:
		return 0;
	case 38:
		return 1;
	case 39:
		return 2;
	case 40:
		return 3;
	case 41:
		return 4;
	case 42:
		return 5;
	case 43:
		return 5;
	case 44:
		return 5;
	case 45:
		return 5;
	default:
		return 0; //break;//throw id;
	}
}

unsigned int SKO_Map::Reader::convertRow(int id)
{
	//TODO make these really work
	switch (id)
	{
	case 0:
		return 0;
	case 1:
		return 0;
	case 2:
		return 0;
	case 3:
		return 1;
	case 4:
		return 1;
	case 5:
		return 1;
	case 6:
		return 2;
	case 7:
		return 2;
	case 8:
		return 2;
	case 9:
		return 0;
	case 10:
		return 0;
	case 11:
		return 1;
	case 12:
		return 1;
	case 13:
		return 2;
	case 14:
		return 2;
	case 15:
		return 3;
	case 16:
		return 3;
	case 17:
		return 3;
	case 18:
		return 4;
	case 19:
		return 4;
	case 20:
		return 4;
	case 21:
		return 5;
	case 22:
		return 5;
	case 23:
		return 5;
	case 24:
		return 3;
	case 25:
		return 3;
	case 26:
		return 4;
	case 27:
		return 4;
	case 28:
		return 4;
	case 29:
		return 5;
	case 30:
		return 6;
	case 31:
		return 6;
	case 32:
		return 6;

	case 33:
		return 0;
	case 34:
		return 0;
	case 35:
		return 0;
	case 36:
		return 0;
	case 37:
		return 1;
	case 38:
		return 1;
	case 39:
		return 1;
	case 40:
		return 1;
	case 41:
		return 0;
	case 42:
		return 0;
	case 43:
		return 0;
	case 44:
		return 0;
	case 45:
		return 0;
	default:
		return 0;//break;//throw id;
	}
}

// TODO - remove this when I'm done with it
SKO_Map::Tile* SKO_Map::Reader::convertTile(int x, int y, int id)
{
	std::string key = convertKey(id);
	unsigned int row = convertRow(id);
	unsigned int column = convertColumn(id);
	return new SKO_Map::Tile(x, y, key, row, column);
}

void SKO_Map::Reader::loadCollisionRects(SKO_Map::Map * map, INIReader mapIni)
{
	// load background tiles
	int num_collision_rects = mapIni.GetInteger("count", "collision_rects", 0);
	printf("num_collision_rects is %i", num_collision_rects);

	for (int i = 0; i < num_collision_rects; i++)
	{
		std::stringstream sspawn_x;
		sspawn_x << "collision_tile_x_" << i;

		std::stringstream sspawn_y;
		sspawn_y << "collision_tile_y_" << i;

		std::stringstream ssw;
		ssw << "collision_tile_w_" << i;

		std::stringstream ssh;
		ssh << "collision_tile_h_" << i;

		int x = mapIni.GetInteger("collision_rects", sspawn_x.str(), 0);
		int y = mapIni.GetInteger("collision_rects", sspawn_y.str(), 0);
		int w = mapIni.GetInteger("collision_rects", ssw.str(), 0);
		int h = mapIni.GetInteger("collision_rects", ssh.str(), 0);

		// Add background tile to map collection
		SDL_Rect collisionRect;
		collisionRect.x = x;
		collisionRect.y = y;
		collisionRect.w = w;
		collisionRect.h = h;

		map->collisionRects.push_back(collisionRect);
	}
}

void SKO_Map::Reader::loadPortals(SKO_Map::Map * map, INIReader mapIni)
{
	// load portals
	int num_portals = mapIni.GetInteger("count", "portals", 0);
	printf("num_portals is %i", num_portals);

	for (int i = 0; i < num_portals; i++)
	{
		std::stringstream ss;
		ss << "portal" << i;

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
		map->portals.push_back(portal);
	}
}

void SKO_Map::Reader::loadSigns(SKO_Map::Map * map, INIReader mapIni)
{
	//load signs
	int num_signs = mapIni.GetInteger("count", "signs", 0);
	printf("num_signs is %i\n", num_signs);

	for (int i = 0; i < num_signs; i++) 
	{
		std::stringstream ss; 
		ss << "sign" << i;    

		SKO_Sign *sign = new SKO_Sign(); 
		sign->x = mapIni.GetInteger(ss.str(), "x", 0); 
		sign->y = mapIni.GetInteger(ss.str(), "y", 0); 
		sign->w = mapIni.GetInteger(ss.str(), "w", 0); 
		sign->h = mapIni.GetInteger(ss.str(), "h", 0); 

		printf("Sign[sign].NUM_LINES is %i\n", SKO_Sign::NUM_LINES);

		//get all the lines of the text
		for (int line = 0; line < SKO_Sign::NUM_LINES; line++) {
			std::stringstream ss1;
			ss1 << "line" << line;
			std::string txt = mapIni.Get(ss.str(), ss1.str(), "");
			if (txt.length())
				txt = txt.substr(1);

			// Add new dialogue lines one at a time
			// TODO - save Font in .ini file for this object
			auto textComponent = new OPI_Text::TextComponent(txt, OPI_Text::FontManager::getFont("RobotoMono-Regular"));
			


			sign->lines.push_back(textComponent);
			printf("%s is: %s\n", ss1.str().c_str(), txt.c_str());
		}

		map->signs.push_back(sign);
	}
}

void SKO_Map::Reader::loadEnemies(SKO_Map::Map * map, INIReader mapIni)
{
	// load enemies
	int num_enemies = mapIni.GetInteger("count", "enemies", 0);
	printf("num_enemies is %i", num_enemies);

	for (int i = 0; i < num_enemies; i++)
	{
		std::stringstream ss;
		ss << "enemy" << i;

		// load portal parameters
		SKO_Enemy * enemy = new SKO_Enemy();
		enemy->type = mapIni.Get(ss.str(), "sprite", ""); // TODO fix with new format, it doesnt match other objects
		enemy->x1 = mapIni.GetInteger(ss.str(), "x1", 0); // TODO fix with new format, it doesnt match other objects
		enemy->x2 = mapIni.GetInteger(ss.str(), "x2", 0); // TODO fix with new format, it doesnt match other objects
		enemy->y1 = mapIni.GetInteger(ss.str(), "y1", 0); // TODO fix with new format, it doesnt match other objects
		enemy->y2 = mapIni.GetInteger(ss.str(), "y2", 0); // TODO fix with new format, it doesnt match other objects
		
		enemy->spawn_x = mapIni.GetInteger(ss.str(), "spawn_x", 0);
		enemy->spawn_y = mapIni.GetInteger(ss.str(), "spawn_y", 0);

		enemy->hp_max = mapIni.GetInteger(ss.str(), "hp", 0);
		enemy->strength = mapIni.GetInteger(ss.str(), "strength", 0);
		enemy->defense = mapIni.GetInteger(ss.str(), "defense", 0);
		enemy->xp = mapIni.GetInteger(ss.str(), "xp", 0);
		
		enemy->lootAmount = mapIni.GetInteger(ss.str(), "loots_dropped", 0);

		int loot_count = mapIni.GetInteger(ss.str(), "loot_count", 0);

		for (int loot = 0; loot < loot_count; loot++)
		{
			std::stringstream ss1;
			ss1 << "loot" << loot << "_item";

			std::stringstream ss2;
			ss2 << "loot" << loot << "_amount";

			std::stringstream ss3;
			ss3 << "loot" << loot << "_chance";

			int loot_item =	mapIni.GetInteger(ss.str(), ss1.str(), 0);
			int loot_amount = mapIni.GetInteger(ss.str(), ss2.str(), 0);
			int loot_chance = mapIni.GetInteger(ss.str(), ss3.str(), 0);

			enemy->addLoot(loot_item, loot_amount, loot_chance);
		}

		enemy->x = enemy->spawn_x;
		enemy->y = enemy->spawn_y;
		enemy->hp = enemy->hp_max;

		// Add portal to map collection
		map->enemies.push_back(enemy);
	}
}

void SKO_Map::Reader::loadStalls(SKO_Map::Map * map, INIReader mapIni)
{
	//load shops
	int num_stalls = mapIni.GetInteger("count", "stalls", 0);
	printf("num_stalls is %i\n", num_stalls);

	for (int i = 0; i < num_stalls; i++) 
	{
		std::stringstream ss1;
		ss1 << "stall" << i;
		std::string stallStr = ss1.str();

		SKO_Stall * stall = new SKO_Stall();
		stall->type = mapIni.Get(stallStr, "shopId", "bank");// TODO separate out into content file: shops.ini
		stall->x = mapIni.GetInteger(stallStr, "x", 0);
		stall->y = mapIni.GetInteger(stallStr, "y", 0);
		stall->w = mapIni.GetInteger(stallStr, "w", 0);
		stall->h = mapIni.GetInteger(stallStr, "h", 0);

		map->stalls.push_back(stall);
	}
}

void SKO_Map::Reader::loadShops(SKO_Map::Map * map, INIReader mapIni)
{
	//load shops
	int num_shops = mapIni.GetInteger("count", "shops", 0);
	printf("num_shops is %i\n", num_shops);

	// Why does this for loop use <= num_shops? Because shop0 doesn't ever exist; it refers to the bank.
	for (int i = 0; i < num_shops; i++)
	{
		SKO_Shop *shop = new SKO_Shop();
		std::stringstream ss1;
		ss1 << "shop" << i;
		std::string shopStr = ss1.str();

		//loop all the X Y places
		for (int x = 0; x < 6; x++)
		{
			for (int y = 0; y < 4; y++)
			{
				std::stringstream ss2, ss3;
				ss2 << "item" << "_" << "x" << x << "_" << "y" << y;
				ss3 << "price" << "_" << "x" << x << "_" << "y" << y;

				//x  y  (item[0], price[1])
				std::string itemStr = ss2.str().c_str();
				std::string priceStr = ss3.str().c_str();

				//now load from config file the items and prices
				shop->item[x][y][0]
					= mapIni.GetInteger(shopStr.c_str(), itemStr.c_str(), 0);
				shop->item[x][y][1]
					= mapIni.GetInteger(shopStr.c_str(), priceStr.c_str(), 0);
			}
		}
		//in the future, maybe a shop title etc. TODO

		map->shops.push_back(shop);
	}
}

void SKO_Map::Reader::loadTargets(SKO_Map::Map * map, INIReader mapIni)
{
	//load targets
	int num_targets = mapIni.GetInteger("count", "targets", 0);
	printf("num_targets is %i\n", num_targets);

	for (int i = 0; i < num_targets; i++) {
		std::stringstream ss1;
		ss1 << "target" << i;
		std::string targetStr = ss1.str();

		SKO_Target *target = new SKO_Target();
		target->picId = mapIni.GetInteger(targetStr, "sprite", 0);
		target->x = mapIni.GetInteger(targetStr, "x", 0);
		target->y = mapIni.GetInteger(targetStr, "y", 0);
		target->w = mapIni.GetInteger(targetStr, "w", 0);
		target->h = mapIni.GetInteger(targetStr, "h", 0);
		map->targets.push_back(target);
	}
}

void SKO_Map::Reader::loadNpcs(SKO_Map::Map * map, INIReader mapIni)
{
	//load npcs
	int num_npcs = mapIni.GetInteger("count", "npcs", 0);
	printf("num_npcs is %i\n", num_npcs);

	for (int i = 0; i < num_npcs; i++) 
	{
		std::stringstream ss1;
		ss1 << "npc" << i;
		std::string targetStr = ss1.str();

		SKO_Npc * npc = new SKO_Npc();
		npc->sprite = mapIni.GetInteger(targetStr, "sprite", 0);
		npc->x = npc->spawn_x = mapIni.GetInteger(targetStr, "x", 0);
		npc->y = npc->spawn_y = mapIni.GetInteger(targetStr, "y", 0);
		npc->finalPage = mapIni.Get(targetStr, "final", "");
		int num_pages = mapIni.GetInteger(targetStr, "pages", 0);
		npc->quest = mapIni.GetInteger(targetStr, "quest", -1); //-1 for non quest NPCs

		//get all the lines of the text
		for (int page = 0; page < num_pages; page++)
		{
			// Add new page to NPC
			npc->pages.push_back(new DialogPage());

			for (int line = 0; line < 10; line++) // TODO change this to use number of lines
			{
				std::stringstream ss1;
				ss1 << "page_" << page << "_";
				ss1 << "line_" << line;
				std::string txt = mapIni.Get(targetStr, ss1.str(), "");
				if (txt.length())
					txt = txt.substr(1);

				// Add new dialogue lines one at a time
				auto textComponent = new OPI_Text::TextComponent(txt, OPI_Text::FontManager::getFont("RobotoMono-Regular"));
				npc->pages[page]->lines.push_back(textComponent);

				printf("NPC %s is: %s\n", ss1.str().c_str(), txt.c_str());
			}
		}

		map->npcs.push_back(npc);
	}
}

