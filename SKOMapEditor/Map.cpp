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
	//open map config file
	printf("Reading Map config from: %s\n", filePath.c_str());
	INIReader configFile(filePath);
	if (configFile.ParseError() < 0) {
		printf("error: Can't load '%s'\n", filePath.c_str());
		return;
	}

	//load portals
	// note: portals are server-side
	// 	  so they don't load on the client
	//load enemies
	int num_enemies = configFile.GetInteger("count", "enemies", 0);
	printf("num_enemies is %i\n", num_enemies);
	for (int enemy = 0; enemy < num_enemies; enemy++) {
		std::stringstream ss;
		ss << "enemy" << enemy;

		//load sprite type from file
		std::string spriteType = configFile.Get(ss.str(), "sprite", "");
		printf("spriteType is %s\n", spriteType.c_str());
		Enemy[enemy] = SKO_Enemy(spriteType);
	}

	//load signs
	num_signs = configFile.GetInteger("count", "signs", 0);
	printf("num_signs is %i\n", num_signs);

	for (int sign = 0; sign < num_signs; sign++) {
		std::stringstream ss;
		ss << "sign" << sign;

		Sign[sign] = SKO_Sign();
		Sign[sign].x = configFile.GetInteger(ss.str(), "x", 0);
		Sign[sign].y = configFile.GetInteger(ss.str(), "y", 0);
		Sign[sign].w = configFile.GetInteger(ss.str(), "w", 0);
		Sign[sign].h = configFile.GetInteger(ss.str(), "h", 0);

		printf("Sign[sign].NUM_LINES is %i\n", Sign[sign].NUM_LINES);

		//get all the lines of the text
		for (int line = 0; line < Sign[sign].NUM_LINES; line++) {
			std::stringstream ss1;
			ss1 << "line" << line;
			std::string txt = configFile.Get(ss.str(), ss1.str(), "");
			if (txt.length())
				txt = txt.substr(1);
			Sign[sign].line[line].SetText(txt.c_str());
			printf("%s is: %s\n", ss1.str().c_str(), txt.c_str());
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
		Target[i].pic = configFile.GetInteger(targetStr, "sprite", 0);
		Target[i].x = configFile.GetInteger(targetStr, "x", 0);
		Target[i].y = configFile.GetInteger(targetStr, "y", 0);
		Target[i].w = configFile.GetInteger(targetStr, "w", 0);
		Target[i].h = configFile.GetInteger(targetStr, "h", 0);
	}

	//load npcs
	num_npcs = configFile.GetInteger("count", "npcs", 0);
	printf("num_npcs is %i\n", num_npcs);

	for (int i = 0; i < num_npcs; i++) {
		std::stringstream ss1;
		ss1 << "npc" << i;
		std::string targetStr = ss1.str();

		NPC[i] = SKO_NPC();
		NPC[i].sprite = configFile.GetInteger(targetStr, "sprite", 0);
		NPC[i].x = NPC[i].sx = configFile.GetInteger(targetStr, "x", 0);
		NPC[i].y = NPC[i].sy = configFile.GetInteger(targetStr, "y", 0);
		NPC[i].final = configFile.GetInteger(targetStr, "final", 0);
		NPC[i].num_pages = configFile.GetInteger(targetStr, "pages", 0);
		NPC[i].quest = configFile.GetInteger(targetStr, "quest", -1); //-1 for non quest NPCs

		//get all the lines of the text
		for (int page = 0; page < NPC[i].num_pages; page++)
			for (int line = 0; line < NPC[i].NUM_LINES; line++)
			{
				std::stringstream ss1;
				ss1 << "page_" << page << "_";
				ss1 << "line_" << line;
				std::string txt = configFile.Get(targetStr, ss1.str(), "");
				if (txt.length())
					txt = txt.substr(1);
				NPC[i].line[page][line]->SetText(txt.c_str());
				printf("NPC %s is: %s\n", ss1.str().c_str(), txt.c_str());
			}


	}
}
