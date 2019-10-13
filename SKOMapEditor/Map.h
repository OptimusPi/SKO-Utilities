#ifndef __SKO_MAP_H_
#define __SKO_MAP_H_


#include "SDL_headers.h"
#include <string>
#include <vector>
#include <fstream>

#include "Tile.h"


namespace SKO_Map
{
	class Map
	{
	public:
		Map();
		Map(std::string filePath);

		virtual ~Map();

		std::vector<Tile*> backgroundTiles;
		std::vector<Tile*> fringeTiles;
		std::vector<SDL_Rect*> collisionRects;
		void saveMap();
		void saveMap(std::string filePath);
		void loadMap(std::string filePath);
	private:
		std::string filePath;
	};
}

#endif
