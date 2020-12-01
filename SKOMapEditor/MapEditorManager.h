#ifndef __SKO_MAPEDITORMANAGER_H_
#define __SKO_MAPEDITORMANAGER_H_


#include <string>

#include "SKO_MapTile.h"
#include "SKO_Map.h"
#include "OPI_Renderer.h"
#include "GuiManager.h"
#include "OPI_Clock.h"
#include "MainMenuGui.h"
#include "SKO_MapReader.h"
#include "SKO_MapTileset.h"

namespace SKO_MapEditor
{
	//TODO Singleton
	class Manager
	{
	public:
		Manager(OPI_Renderer * renderer, MainMenuGui *mainMenuGui);
		virtual ~Manager();
		void saveMap();
		void saveMap(std::string filePath);
		void loadMap(std::string filePath);
		// Clean up useless rectangles (<4x4 pixels) to a SKO_Map in-place.
		void cleanupInvisibleRects(SKO_Map::Map *map);
		
		//Clean up accidentally duplicated tiles
		void removeDuplicateTiles(std::vector<SKO_Map::Tile*> *tiles);
		void removeDuplicateTiles(SKO_Map::Map *map);

		// Draw: sky, map layers, and player test object
		void drawTileLayer(int camera_x, int camera_y, std::map<std::string, std::vector<SKO_Map::Tile*>> tileLayer);
		void DrawGameScene(int camera_x, int camera_y);

		// Draw all map editor gui
		void DrawGui();

		// Handle input 
		void HandleInput();

		// Process logic loop
		void processLoop();

		// Load map editor images such as tilesets, stickman, background
		void loadImages();
		void loadTilesets();

	private:
		SKO_Map::Reader *mapReader = nullptr;
		SDL_Event event;
		MainMenuGui *mainMenuGui = nullptr;
		OPI_Renderer * renderer = nullptr;
		SKO_Map::Map * map = nullptr;
		float camera_x = 0, camera_y = 0;
		int cursor_x = 0, cursor_y = 0;
		SKO_Map::Tile *current_tile = nullptr;
		int current_rect = -1;
		
		std::map<std::string, SKO_Map::Tileset*> tilesets;
		std::vector<std::string> tilesetKeys;

		SKO_Map::Tileset* current_tileset = nullptr;
		unsigned int current_tileset_row = 0;
		unsigned int current_tileset_column = 0;

		int num_tile_images = 0;
		int collision_ox = 0;
		int collision_oy = 0;

		//images
		OPI_Image background; //meh
		OPI_Image stickman_img; //meh
		OPI_Gui::GuiManager *gui;

		//mouse buttons
		bool RCLICK = false;
		bool LCLICK = false;

		bool placing_tile = false;
		bool placing_fringe = false;

		int save_notify;
		bool SHIFT_HELD = false;
		bool stickman_toggle = false;
		unsigned long int coordsTicker = 0;

		SDL_Rect stickman;
		float x_speed, y_speed;
		const float GRAVITY = 0.175;
		bool RIGHT = false, LEFT = false;

		// Simulate Physics of test stickman
		bool isBlocked(SDL_Rect rect);
		void physics();
		void graphics();
	};
}

#endif

