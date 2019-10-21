#include "Map.h"


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
			unsigned char *p = (unsigned char*)(backgroundTiles[i]->x);
			unsigned char b1 = p[0]; 
			unsigned char b2 = p[1]; 
			unsigned char b3 = p[2]; 
			unsigned char b4 = p[3];

			//spit out each of the bytes
			MapFile << b1 << b2 << b3 << b4;

			//y coords
			p = (unsigned char*)backgroundTiles[i]->y;
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
			unsigned char *p = (unsigned char*)fringeTiles[i]->x;
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
}

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
			SDL_Rect newRect;
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

			newRect.h = h;
			newRect.w = w;
			newRect.x = x;
			newRect.y = y;
			this->collisionRects.push_back(newRect);
		}

		free(memblock);
	}
}