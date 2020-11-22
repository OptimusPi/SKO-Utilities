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
		Tileset(OPI_Image *image, std::string name, std::string key, unsigned short tile_width, unsigned short tile_height, unsigned char rows, unsigned char columns);
		virtual ~Tileset();

		OPI_Image* image = nullptr;
		std::string name = "";
		std::string key = "00000000-0000-0000-0000-000000000000";
		unsigned short tile_width = 0;
		unsigned short tile_height = 0;
		unsigned char rows = 0;
		unsigned char columns = 0;
	private:

	};
}

#endif