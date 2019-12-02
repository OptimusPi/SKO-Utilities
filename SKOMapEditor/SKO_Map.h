#ifndef __SKO_MAP_H_
#define __SKO_MAP_H_


#include "SDL_headers.h"
#include <string>
#include <vector>
#include <fstream>


// SKO Map objects
#include "SKO_MapTile.h"

// SKO game object Models
#include "SKO_Portal.h"
#include "SKO_Sign.h"
#include "SKO_Enemy.h" 
#include "SKO_Stall.h"
#include "SKO_Shop.h"
#include "SKO_Target.h"
#include "SKO_Npc.h"

namespace SKO_Map
{
	class Map
	{ 
	public:  
		Map(); 
		virtual ~Map(); 

		// Static map objects
		std::vector<Tile*> backgroundTiles;   
		std::vector<Tile*> fringeTiles;       
		std::vector<SDL_Rect> collisionRects; 

		// Living game objects inside this map
		std::vector<SKO_Portal*>portals;
		std::vector<SKO_Sign*>signs;
		std::vector<SKO_Enemy*>enemies;
		std::vector<SKO_Stall*>stalls;
		std::vector<SKO_Shop*>shops;
		std::vector<SKO_Target*>targets;
		std::vector<SKO_Npc*>npcs;

		// TODO - Move to Reader/Writer classes
		void saveMap(); 
		void saveMap(std::string filePath); 
		void loadMap(std::string filePath); 
		
		// New version to save as INI and read INI
		void saveMapINI(std::string filePath); 
		void loadMapINI(std::string filePath); 
		std::string filePath;
	private:
		
	};
}

#endif
