#ifndef __SKO_MAP_H_
#define __SKO_MAP_H_


#include "SDL_headers.h"
#include <string>
#include <vector>
#include <iostream>
#include "Tile.h"


namespace SKO_Map
{
	class Map
	{
	public:
		Map();
		Map(std::string filePath);

		virtual ~Map();

		std::vector<Tile*> backGroundTiles;
		std::vector<Tile*> fringeTiles;
		std::vector<SDL_Rect*> collisionRects;
		void saveMap();
		void saveMap(std::string fileName);
		void loadMap(std::string fileName);
		int collision_ox = 0;
		int collision_oy = 0;// TODO rename
	private:
		std::string filePath;
	};
}

#endif
