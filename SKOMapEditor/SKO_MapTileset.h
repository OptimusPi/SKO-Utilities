#include <string>
#ifndef __SKOMAPTILESET_H_
#define __SKOMAPTILESET_H_

namespace SKO_Map
{
	class Tileset
	{
	public:
		Tileset(std::string filePath = "DAT/tilesets.ini");
		virtual ~Tileset();

		std::string filePath = "";
	private:

	};
}

#endif