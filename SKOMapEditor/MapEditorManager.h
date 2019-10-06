#ifndef __SKO_MAPEDITORMANAGER_H_
#define __SKO_MAPEDITORMANAGER_H_


#include "Map.h"
#include <string>


namespace SKO_MapEditor
{
	//TODO Singleton
	class Manager
	{
	public:
		Manager();
		virtual ~Manager();
		void saveMap();
		void saveMap(std::string filename);
		void loadMap(std::string filename);
		// Clean up useless rectangles (<4x4 pixels) to a SKO_Map in-place.
		void cleanupInvisibleRects(SKO_Map::Map *map);
	private:
		SKO_Map::Map *map;
		float camera_x = 0, camera_y = 0;
		int cursor_x = 0, cursor_y = 0;
		int current_tile = 0;
		int current_rect = 0;
		int current_fringe = 0;
		int current_tile_img = 0;
	};
}

#endif

