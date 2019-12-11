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


namespace SKO_MapEditor
{
	//TODO Singleton
	class Manager
	{
	public:
		Manager(OPI_Renderer * renderer, MainMenuGui *mainMenuGui, SKO_Map::Reader *mapReader);
		virtual ~Manager();
		void saveMap();
		void saveMap(std::string filename);
		void loadMap(std::string filename);
		// Clean up useless rectangles (<4x4 pixels) to a SKO_Map in-place.
		void cleanupInvisibleRects(SKO_Map::Map *map);

		//Clean up accidentally duplicated tiles
		void removeDuplicateTiles(std::vector<SKO_Map::Tile*> *tiles);
		void removeDuplicateTiles(SKO_Map::Map *map);

		// Draw: sky, map layers, and player test object
		void DrawGameScene(int camera_x, int camera_y);

		// Draw all map editor gui
		void DrawGui();

		// Handle input 
		void HandleInput();

		// Process logic loop
		void processLoop();

	private:
		SDL_Event event;
		MainMenuGui *mainMenuGui;
		OPI_Renderer * renderer;
		SKO_Map::Map * map;
		SKO_Map::Reader * mapReader;
		float camera_x = 0, camera_y = 0;
		int cursor_x = 0, cursor_y = 0;
		int current_tile = 0;
		int current_rect = 0;
		int current_fringe = 0;
		int current_tile_img = 0;
		int num_tile_images = 0;
		int collision_ox = 0;
		int collision_oy = 0;

		//images
		OPI_Image tile_img[256];
		OPI_Image background;
		OPI_Image stickman_img;
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

