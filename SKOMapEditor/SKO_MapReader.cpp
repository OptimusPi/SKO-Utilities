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

	// TODO loadNatureEngine();

	return map;
}
void SKO_Map::Reader::loadBackgroundTiles(SKO_Map::Map * map, INIReader mapIni)
{
	// load background tiles
	int num_background_tiles = mapIni.GetInteger("count", "background_tiles", 0);
	printf("num_background_tiles is %i", num_background_tiles);

	for (int i = 0; i < num_background_tiles; i++)
	{
		std::stringstream section;
		section << "background_tile" << i;


		int x = mapIni.GetInteger(section.str(), "x", 0);
		int y = mapIni.GetInteger(section.str(), "y", 0);
		int id = mapIni.GetInteger(section.str(), "tile_id", 0);

		// Add background tile to map collection
		SKO_Map::Tile *backgroundTile = new SKO_Map::Tile(x, y, id);
		map->backgroundTiles.push_back(backgroundTile);
	}
}

void SKO_Map::Reader::loadFringeTiles(SKO_Map::Map * map, INIReader mapIni)
{
	// load fringe tiles
	int num_fringe_tiles = mapIni.GetInteger("count", "fringe_tiles", 0);
	printf("num_fringe_tiles is %i", num_fringe_tiles);

	for (int i = 0; i < num_fringe_tiles; i++)
	{
		std::stringstream sspawn_x;
		sspawn_x << "x" << i;

		std::stringstream sspawn_y;
		sspawn_y << "y" << i;

		std::stringstream ssid;
		ssid << "tile_id" << i;

		int x = mapIni.GetInteger("fringe_tiles", sspawn_x.str(),  0);
		int y = mapIni.GetInteger("fringe_tiles", sspawn_y.str(),  0);
		int id = mapIni.GetInteger("fringe_tiles", ssid.str(), 0);

		// Add fringe tile to map collection
		SKO_Map::Tile *fringeTile = new SKO_Map::Tile(x, y, id);
		map->fringeTiles.push_back(fringeTile);
	}
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

