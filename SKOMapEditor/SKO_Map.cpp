#include "Map.h"
#include "INIReader.h" // TODO - make class for map saving/loading
#include <fstream> // TODO - make class for map saving/loading
#include <sstream>

SKO_Map::Map::Map()
{
}

SKO_Map::Map::Map(std::string filePath)
{
	loadMap(filePath);
}

SKO_Map::Map::~Map()
{

}

void SKO_Map::Map::saveMap()
{
	this->saveMap(this->filePath);
}

void SKO_Map::Map::saveMap(std::string filePath)
{
	//dump all the memory into a file
	std::ofstream MapFile(filePath, std::ios::out | std::ios::binary);
	int number_of_tiles = this->backgroundTiles.size();
	int number_of_fringe = this->fringeTiles.size();
	int number_of_rects = this->collisionRects.size();

	if (MapFile.is_open())
 	{ 
		//first say how many tiles
		//break up the int as 4 bytes 
		unsigned char *p;
		p = (unsigned char*)&number_of_tiles;
		unsigned char b1 = p[0];
		unsigned char b2 = p[1];
		unsigned char b3 = p[2];
		unsigned char b4 = p[3];

		//spit out each of the bytes
		MapFile << b1 << b2 << b3 << b4;

		//put how many fringe
		p = (unsigned char*)&number_of_fringe;
		b1 = p[0]; b2 = p[1]; b3 = p[2]; b4 = p[3];

		//spit out each of the bytes
		MapFile << b1 << b2 << b3 << b4;

		//put how many rects
		p = (unsigned char*)&number_of_rects;
		b1 = p[0]; b2 = p[1]; b3 = p[2]; b4 = p[3];

		//spit out each of the bytes
		MapFile << b1 << b2 << b3 << b4;

		//spit out all the tiles
		for (int i = 0; i < number_of_tiles; i++)
		{
			//x coords
			unsigned char *p = (unsigned char*)(&backgroundTiles[i]->x);
			unsigned char b1 = p[0]; 
			unsigned char b2 = p[1]; 
			unsigned char b3 = p[2]; 
			unsigned char b4 = p[3];

			//spit out each of the bytes
			MapFile << b1 << b2 << b3 << b4;

			//y coords
			p = (unsigned char*)(&backgroundTiles[i]->y);
			b1 = p[0]; b2 = p[1]; b3 = p[2]; b4 = p[3];

			//spit out each of the bytes
			MapFile << b1 << b2 << b3 << b4;

			//tile number
			MapFile << backgroundTiles[i]->tileId;
		}

		//spit out all the tiles
		for (int i = 0; i < number_of_fringe; i++)
		{
			//x coords
			unsigned char *p = (unsigned char*)&fringeTiles[i]->x;
			unsigned char b1 = p[0]; 
			unsigned char b2 = p[1]; 
			unsigned char b3 = p[2]; 
			unsigned char b4 = p[3];

			//spit out each of the bytes
			MapFile << b1 << b2 << b3 << b4;

			//y coords
			p = (unsigned char*)&fringeTiles[i]->y;
			b1 = p[0]; b2 = p[1]; b3 = p[2]; b4 = p[3];

			//spit out each of the bytes
			MapFile << b1 << b2 << b3 << b4;

			//fringe number
			MapFile << fringeTiles[i]->tileId;

		}

		//spit out all the rects
		for (int i = 0; i < number_of_rects; i++)
		{
			//x coords
			int x = collisionRects[i].x;
			unsigned char *p = (unsigned char*)&x;
			unsigned char b1 = p[0];
			unsigned char b2 = p[1];
			unsigned char b3 = p[2];
			unsigned char b4 = p[3];

			//spit out each of the bytes
			MapFile << b1 << b2 << b3 << b4;

			//y coords
			int y = collisionRects[i].y;
			p = (unsigned char*)&y;
			b1 = p[0]; b2 = p[1]; b3 = p[2]; b4 = p[3];

			//spit out each of the bytes
			MapFile << b1 << b2 << b3 << b4;

			//width
			int w = collisionRects[i].w;
			p = (unsigned char*)&w;
			b1 = p[0]; b2 = p[1]; b3 = p[2]; b4 = p[3];

			//spit out each of the bytes
			MapFile << b1 << b2 << b3 << b4;

			//height
			int h = collisionRects[i].h;
			p = (unsigned char*)&h;
			b1 = p[0]; b2 = p[1]; b3 = p[2]; b4 = p[3];

			//spit out each of the bytes
			MapFile << b1 << b2 << b3 << b4;
		}

		MapFile.close();
	}

	saveMapINI(filePath + ".ini");
}

// TODO load from INI
// DEPRECATED
void SKO_Map::Map::loadMap(std::string filePath)
{
	std::ifstream MapFile(filePath, std::ios::in | std::ios::binary | std::ios::ate);
	this->filePath = filePath;

	if (MapFile.is_open())
	{
		//loading variables
		std::ifstream::pos_type size;
		char * memblock;    

		//allocate memory
		size = MapFile.tellg();
		memblock = (char *)malloc(size);

		//load the file into memory      
		MapFile.seekg(0, std::ios::beg); 
		MapFile.read(memblock, size);    
		//close file                     
		MapFile.close();                 

		//hold the result... 
		unsigned int num;    

		//build an int from 4 bytes
		((char*)&num)[0] = memblock[0];
		((char*)&num)[1] = memblock[1];
		((char*)&num)[2] = memblock[2];
		((char*)&num)[3] = memblock[3]; 


		//store the number into variables
		int number_of_tiles = num;

		//build an int from 4 bytes
		((char*)&num)[0] = memblock[4];
		((char*)&num)[1] = memblock[5];
		((char*)&num)[2] = memblock[6];
		((char*)&num)[3] = memblock[7];

		//store the number into variables
		int number_of_fringe = num;

		//build an int from 4 bytes
		((char*)&num)[0] = memblock[8];
		((char*)&num)[1] = memblock[9];
		((char*)&num)[2] = memblock[10];
		((char*)&num)[3] = memblock[11];

		//store the number into variables
		int number_of_rects = num;


		//
		//tiles
		//
		int last_i = 11;

		for (int i = 0; i < number_of_tiles; i++)
		{
			//9 bytes per tile silly ;)

			//build an int from 4 bytes
			((char*)&num)[0] = memblock[last_i + 1 + i * 9];
			((char*)&num)[1] = memblock[last_i + 2 + i * 9];
			((char*)&num)[2] = memblock[last_i + 3 + i * 9];
			((char*)&num)[3] = memblock[last_i + 4 + i * 9];

			//store the number into variables
			int x = num;

			//build an int from 4 bytes
			((char*)&num)[0] = memblock[last_i + 5 + i * 9];
			((char*)&num)[1] = memblock[last_i + 6 + i * 9];
			((char*)&num)[2] = memblock[last_i + 7 + i * 9];
			((char*)&num)[3] = memblock[last_i + 8 + i * 9];

			//store the number into variables
			int y = num;

			//store the number into variables
			int tileId = (unsigned char)memblock[last_i + 9 + i * 9];

			this->backgroundTiles.push_back(new SKO_Map::Tile(x, y, tileId));
		}

		last_i += number_of_tiles * 9;
		//
		//fringe tiles
		//
		for (int i = 0; i < number_of_fringe; i++)
		{
			//9 bytes per tile silly ;)

			//build an int from 4 bytes
			((char*)&num)[0] = memblock[last_i + 1 + i * 9];
			((char*)&num)[1] = memblock[last_i + 2 + i * 9];
			((char*)&num)[2] = memblock[last_i + 3 + i * 9];
			((char*)&num)[3] = memblock[last_i + 4 + i * 9];

			//store the number into variables
			int x = num;


			//build an int from 4 bytes
			((char*)&num)[0] = memblock[last_i + 5 + i * 9];
			((char*)&num)[1] = memblock[last_i + 6 + i * 9];
			((char*)&num)[2] = memblock[last_i + 7 + i * 9];
			((char*)&num)[3] = memblock[last_i + 8 + i * 9];

			//store the number into variables
			int y = num;

			//store the number into variables
			int tileId = memblock[last_i + 9 + i * 9];

			this->fringeTiles.push_back(new SKO_Map::Tile(x, y, tileId));
		}
		last_i += number_of_fringe * 9;
		//
		//rects
		//
		for (int i = 0; i < number_of_rects; i++)
		{
			//read the map file
			//build an int from 4 bytes
			((char*)&num)[0] = memblock[last_i + 1 + i * 16];
			((char*)&num)[1] = memblock[last_i + 2 + i * 16];
			((char*)&num)[2] = memblock[last_i + 3 + i * 16];
			((char*)&num)[3] = memblock[last_i + 4 + i * 16];

			//store the number into variables
			int x = num;

			//build an int from 4 bytes
			((char*)&num)[0] = memblock[last_i + 5 + i * 16];
			((char*)&num)[1] = memblock[last_i + 6 + i * 16];
			((char*)&num)[2] = memblock[last_i + 7 + i * 16];
			((char*)&num)[3] = memblock[last_i + 8 + i * 16];

			//store the number into variables
			int y = num;

			//build an int from 4 bytes
			((char*)&num)[0] = memblock[last_i + 9  + i * 16];
			((char*)&num)[1] = memblock[last_i + 10 + i * 16];
			((char*)&num)[2] = memblock[last_i + 11 + i * 16];
			((char*)&num)[3] = memblock[last_i + 12 + i * 16];

			//store the number into variables
			int w = num;

			//build an int from 4 bytes
			((char*)&num)[0] = memblock[last_i + 13 + i * 16];
			((char*)&num)[1] = memblock[last_i + 14 + i * 16];
			((char*)&num)[2] = memblock[last_i + 15 + i * 16];
			((char*)&num)[3] = memblock[last_i + 16 + i * 16];


			//store the number into variables
			int h = num;

			SDL_Rect newRect;
			newRect.x = (unsigned short int)x;
			newRect.y = (unsigned short int)y;
			newRect.h = (short int)h;
			newRect.w = (short int)w;
			this->collisionRects.push_back(newRect);
		}

		free(memblock);
	}
}



void SKO_Map::Map::saveMapINI(std::string filePath)
{
	const int VERSION_MAJOR = 1;
	const int VERSION_MINOR = 0;

	//open the file 
	std::ofstream mapFile(filePath);

	// Title and version
	mapFile << "# Stick Knights Online Map File" << std::endl;
	mapFile << std::endl;
	mapFile << "[version]" <<std::endl;
	mapFile << "version = " << VERSION_MAJOR << "." << VERSION_MINOR << std::endl;
	mapFile << std::endl;

	//output the counts of everything: tiles & collisions
	mapFile << "[count]" << std::endl;
	mapFile << "background_tiles = " << this->backgroundTiles.size() << std::endl;
	mapFile << "fringe_tiles = " << this->fringeTiles.size() << std::endl;
	mapFile << "collision_rects = " << this->collisionRects.size() << std::endl;
	mapFile << std::endl;

	//output the background tiles
	mapFile << "[background_tiles]" << std::endl;
	for (int i = 0; i < this->backgroundTiles.size(); i++)
	{
		mapFile << "background_tile_x_" << i << " = " << this->backgroundTiles[i]->x << std::endl;
		mapFile << "background_tile_y_" << i << " = " << this->backgroundTiles[i]->y << std::endl;
		mapFile << "background_tile_id_" << i << " = " << (int)this->backgroundTiles[i]->tileId << std::endl;
		mapFile << std::endl;
	}
	mapFile << std::endl;

	//output the fringe tiles
	mapFile << "[fringe_tiles]" << std::endl;
	for (int i = 0; i < this->fringeTiles.size(); i++)
	{
		mapFile << "fringe_tile_x_" << i << " = " << this->fringeTiles[i]->x << std::endl;
		mapFile << "fringe_tile_y_" << i << " = " << this->fringeTiles[i]->y << std::endl;
		mapFile << "fringe_tile_id_" << i << " = " << (int)this->fringeTiles[i]->tileId << std::endl;
		mapFile << std::endl;
	}
	mapFile << std::endl;

	//output the collision rectangles
	mapFile << "[collision_rects]" << std::endl;
	for (int i = 0; i < this->collisionRects.size(); i++)
	{
		mapFile << "collision_tile_x_" << i << " = " << this->collisionRects[i].x << std::endl;
		mapFile << "collision_tile_y_" << i << " = " << this->collisionRects[i].y << std::endl;
		mapFile << "collision_tile_w_" << i << " = " << this->collisionRects[i].w << std::endl;
		mapFile << "collision_tile_h_" << i << " = " << this->collisionRects[i].h << std::endl;

		mapFile << std::endl;
	}
	mapFile << std::endl;




}

void SKO_Map::Map::loadMapINI(std::string filePath)
{
	//load enemies
	int num_enemies = configFile.GetInteger("count", "enemies", 0);
	printf("num_enemies is %i\n", num_enemies);
	for (int enemy = 0; enemy < num_enemies; enemy++) {
		std::stringstream ss;
		ss << "enemy" << enemy;

		//load sprite type from file
		Enemy[enemy] = new SKO_Enemy();
		Enemy[enemy]->x1 = configFile.GetInteger(ss.str(), "x1", 0);
		Enemy[enemy]->x2 = configFile.GetInteger(ss.str(), "x2", 0);
		Enemy[enemy]->y1 = configFile.GetInteger(ss.str(), "y1", 0);
		Enemy[enemy]->y2 = configFile.GetInteger(ss.str(), "y2", 0);

		Enemy[enemy]->sx = configFile.GetInteger(ss.str(), "spawn_x", 0);
		Enemy[enemy]->sy = configFile.GetInteger(ss.str(), "spawn_y", 0);
		Enemy[enemy]->x = Enemy[enemy]->sx;
		Enemy[enemy]->y = Enemy[enemy]->sy;

		Enemy[enemy]->hp_max = configFile.GetInteger(ss.str(), "hp", 0);
		Enemy[enemy]->hp = Enemy[enemy]->hp_max;
		Enemy[enemy]->strength = configFile.GetInteger(ss.str(), "strength", 0);
		Enemy[enemy]->defense = configFile.GetInteger(ss.str(), "defense", 0);
		Enemy[enemy]->xp = configFile.GetInteger(ss.str(), "xp", 0);

		Enemy[enemy]->lootAmount = configFile.GetInteger(ss.str(), "loots_dropped", 0);

		int loot_count = configFile.GetInteger(ss.str(), "loot_count", 0);

		for (int loot = 0; loot < loot_count; loot++)
		{
			std::stringstream ss1;
			ss1 << "loot" << loot << "_item";

			std::stringstream ss2;
			ss2 << "loot" << loot << "_amount";

			std::stringstream ss3;
			ss3 << "loot" << loot << "_chance";

			int loot_item = configFile.GetInteger(ss.str(), ss1.str(), 0);
			int loot_amount = configFile.GetInteger(ss.str(), ss2.str(), 0);
			int loot_chance = configFile.GetInteger(ss.str(), ss3.str(), 0);


			Enemy[enemy]->addLoot(loot_item, loot_amount, loot_chance);


		}
	}

	//load shops
	num_shops = configFile.GetInteger("count", "shops", 0);
	printf("num_shops is %i\n", num_shops);

	//start at i= 1 ... why?
	//  because ID 0 is reserved for the bank
	for (int i = 0; i <= num_shops; i++) {
		Shop[i] = SKO_Shop();
		std::stringstream ss1;
		ss1 << "shop" << i;
		std::string shopStr = ss1.str();
		//loop all the X Y places
		for (int x = 0; x < 6; x++) {
			for (int y = 0; y < 4; y++) {
				std::stringstream ss2, ss3;
				ss2 << "item" << "_" << "x" << x << "_" << "y" << y;
				ss3 << "price" << "_" << "x" << x << "_" << "y" << y;

				//x  y  (item[0], price[1])
				std::string itemStr = ss2.str().c_str();
				std::string priceStr = ss3.str().c_str();

				//now load from config file the items and prices
				Shop[i].item[x][y][0]
					= configFile.GetInteger(shopStr.c_str(), itemStr.c_str(), 0);
				Shop[i].item[x][y][1]
					= configFile.GetInteger(shopStr.c_str(), priceStr.c_str(), 0);
			}
		}

		//all done loading shops for now.
		//in the future, maybe a shop title etc.
	}

	//load stalls
	num_stalls = configFile.GetInteger("count", "stalls", 0);
	printf("num_stalls is %i\n", num_stalls);

	for (int i = 0; i < num_stalls; i++) {
		std::stringstream ss1;
		ss1 << "stall" << i;
		std::string stallStr = ss1.str();

		Stall[i] = SKO_Stall();
		Stall[i].shopId = configFile.GetInteger(stallStr, "shopId", 0);
		Stall[i].x = configFile.GetInteger(stallStr, "x", 0);
		Stall[i].y = configFile.GetInteger(stallStr, "y", 0);
		Stall[i].w = configFile.GetInteger(stallStr, "w", 0);
		Stall[i].h = configFile.GetInteger(stallStr, "h", 0);
	}

	//load targets
	num_targets = configFile.GetInteger("count", "targets", 0);
	printf("num_targets is %i\n", num_targets);

	for (int i = 0; i < num_targets; i++) {
		std::stringstream ss1;
		ss1 << "target" << i;
		std::string targetStr = ss1.str();

		Target[i] = SKO_Target();
		Target[i].x = configFile.GetInteger(targetStr, "x", 0);
		Target[i].y = configFile.GetInteger(targetStr, "y", 0);
		Target[i].w = configFile.GetInteger(targetStr, "w", 0);
		Target[i].h = configFile.GetInteger(targetStr, "h", 0);
		Target[i].loot = configFile.GetInteger(targetStr, "loot", 0);
		printf("Target[%i]: (%i,%i)\n", i, Target[i].x, Target[i].y);
	}

	//load npcs
	num_npcs = configFile.GetInteger("count", "npcs", 0);
	printf("num_npcs is %i\n", num_npcs);

	for (int i = 0; i < num_npcs; i++) {
		std::stringstream ss1;
		ss1 << "npc" << i;
		std::string npcStr = ss1.str();

		NPC[i] = new SKO_NPC();
		NPC[i]->sx = NPC[i]->x = configFile.GetInteger(npcStr, "x", 0);
		NPC[i]->sy = NPC[i]->y = configFile.GetInteger(npcStr, "y", 0);
		printf("NPC[%i]: (%i,%i)\n", i, (int)NPC[i]->x, (int)NPC[i]->y);
	}


	//when things fall this low they are erased or respawned
	death_pit = configFile.GetInteger("map", "death_pit", 10000);
	printf("death_pit is: %i\n", (int)death_pit);

	spawn_x = configFile.GetInteger("map", "spawn_x", 0);
	printf("spawn_x is: %i\n", spawn_x);

	spawn_y = configFile.GetInteger("map", "spawn_y", 0);
	printf("spawn_y is: %i\n", spawn_y);

	}
}
