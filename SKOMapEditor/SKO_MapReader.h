#ifndef __SKO_MAP_READER_H_
#define __SKO_MAP_READER_H_

#include "SKO_Map.h"
#include <string>

namespace SKO_Map
{
	class Reader
	{
	public:
		static SKO_Map::Map *loadMap(std::string filePath);
	private:
		
	};
}

#endif