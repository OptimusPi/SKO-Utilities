#include <string>
#include <map>

#include "OPI_Image.h"
#ifndef __SKOMAPTILESET_H_
#define __SKOMAPTILESET_H_

namespace SKO_Map
{
	class Tileset
	{
	public:
		Tileset(OPI_Image *image, std::string name, std::string key, unsigned char tile_width, unsigned char tile_height, unsigned char rows, unsigned char columns);
		virtual ~Tileset();
		OPI_Image* getTileImage(unsigned int row, unsigned int column);
		std::string getTileKey(unsigned int row, unsigned int column);

		OPI_Image* image = nullptr;
		std::map<std::string, OPI_Image*> tileImages;
		std::string name = "";
		std::string key = "00000000-0000-0000-0000-000000000000";
		unsigned char tile_width = 0;
		unsigned char tile_height = 0;
		unsigned char rows = 0;
		unsigned char columns = 0;
	private:

	};
}

#endif