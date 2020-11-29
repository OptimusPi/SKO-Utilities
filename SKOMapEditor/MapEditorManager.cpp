#include "MapEditorManager.h"

#include "Global.h"
#include <set>


SKO_MapEditor::Manager::Manager(OPI_Renderer * renderer, MainMenuGui *mainMenuGui, SKO_Map::Reader *mapReader)
{
	this->renderer = renderer;
	this->mainMenuGui = mainMenuGui;
	this->gui = OPI_Gui::GuiManager::getInstance();
	this->map = new SKO_Map::Map();
	this->mapReader = mapReader;
	this->loadImages();
	this->current_tileset = tilesets[tilesetKeys[0]];
}

SKO_MapEditor::Manager::~Manager()
{

}
 
void SKO_MapEditor::Manager::loadImages()
{
	this->background.setImage("IMG/back.png");
	this->stickman_img.setImage("IMG/stickman.png");

	// load tilesets
	this->loadTilesets();

	// 

	//// Load tileset
	//for (int i = 0; i < 256; i++)//check if file exists, etc.
	//{
	//	std::stringstream ss;
	//	ss << "IMG/TILE/tile" << i << ".png";
	//	std::ifstream checker(ss.str());
	//	if (checker.is_open())
	//	{
	//		checker.close();
	//		tile_img[i].setImage(ss.str());
	//		num_tile_images = i;
	//	}
	//	else
	//		break;
	//}
}

void SKO_MapEditor::Manager::loadTilesets()
{
	std::string filePath = "DAT/tilesets.ini";

	// Open the file to read
	INIReader tilesetIni(filePath);
	if (tilesetIni.ParseError() < 0)
	{
		std::string error = "SKO_MapEditor::Manager::loadTilesets() Failed to load INI file: " + filePath;
		throw new std::ios_base::failure(error);
	}

	// load background tiles
	int tilesetsCount = tilesetIni.GetInteger("tilesets", "count", 0);

	for (int i = 0; i < tilesetsCount; i++)
	{
		std::stringstream ss;
		ss << "tileset" << i;
		std::string name = tilesetIni.Get(ss.str(), "name", "");
		std::string key = tilesetIni.Get(ss.str(), "key", "00000000-0000-0000-0000-000000000000");
		std::string filePath = tilesetIni.Get(ss.str(), "filepath", "");
		unsigned char tile_width = tilesetIni.GetInteger(ss.str(), "tile_width", 32);
		unsigned char tile_height = tilesetIni.GetInteger(ss.str(), "tile_height", 32);
		unsigned char rows = tilesetIni.GetInteger(ss.str(), "rows", 32);
		unsigned char columns = tilesetIni.GetInteger(ss.str(), "columns", 32);

		try
		{
			auto image = new OPI_Image(filePath);
			auto tileset = new SKO_Map::Tileset(image, name, key, tile_width, tile_height, rows, columns);
			tilesets.insert({ tileset->key, tileset });
			tilesetKeys.push_back(tileset->key);
		}
		catch (...)
		{
			continue;
		}
	}

	map->filePath = "MAP/default.map.ini";
}


// Clean up useless rectangles (<4x4 pixels) to a SKO_Map in-place.
void SKO_MapEditor::Manager::cleanupInvisibleRects(SKO_Map::Map *map)
{
	std::vector<int> invisibleRectIds;

	//Delete any collision rectangles that are too small.
	for (int i = 0; i < map->collisionRects.size(); i++)
	{
		// If rect is nearly invisible, remove it
		if (map->collisionRects[i].h < 3 && map->collisionRects[i].w < 3)
		{
			current_rect--;
			invisibleRectIds.push_back(i);
		}
	}	

	for (int i = invisibleRectIds.size() - 1; i > 0; i--)
	{
		map->collisionRects.erase(map->collisionRects.begin() + invisibleRectIds[i]);
	}

	// set current rect to last rect in list
	current_rect = map->collisionRects.size() - 1;
}

void SKO_MapEditor::Manager::removeDuplicateTiles(std::vector<SKO_Map::Tile*> *tiles)
{
	std::vector<int> duplicateTileIds = std::vector<int>();

	// Find duplicate tiles
	for (int i = 0; i < tiles->size(); i++)
	{
		SKO_Map::Tile *tileA = (*tiles)[i];
		for (int i2 = i + 1; i2 < tiles->size(); i2++)
		{
			SKO_Map::Tile *tileB = (*tiles)[i2];

			if (SKO_Map::Tile::same(tileA, tileB))// TODO compare function on SKO_Map::Tile::compare
			{
				// Remove the first tile, because the last tile wins, since it is on top.
				duplicateTileIds.push_back(i);
				break;
			}
		}
	}

	// Remove duplicate tiles from the back of the list to the front
	for (int i = duplicateTileIds.size() - 1; i > 0; i--)
	{
		int at = duplicateTileIds[i];
		tiles->erase(tiles->begin() + at);
	}

}

void SKO_MapEditor::Manager::removeDuplicateTiles(SKO_Map::Map *map)
{
	for (auto it = map->backgroundTiles.begin(); it != map->backgroundTiles.end(); it++)
	{
		auto tilesetKey = it->first;
		auto tiles = it->second;
		removeDuplicateTiles(&tiles);
	}

	for (auto it = map->fringeTiles.begin(); it != map->fringeTiles.end(); it++)
	{
		auto tilesetKey = it->first;
		auto tiles = it->second;
		removeDuplicateTiles(&tiles);
	}
}

 
void SKO_MapEditor::Manager::saveMap(std::string fileLocation)
{
	// Cleanup small rects you can't see
	this->cleanupInvisibleRects(this->map);

	// Save the map at the given file location
	this->map->saveMap(fileLocation, tilesets);
}

void SKO_MapEditor::Manager::saveMap()
{
	// Cleanup small rects you can't see
	this->cleanupInvisibleRects(this->map);

	// Remove duplicate tiles that are exactly the same
	this->removeDuplicateTiles(this->map);

	// Save the map to its previous file location
	this->map->saveMap(this->tilesets);
}


void SKO_MapEditor::Manager::loadMap(std::string fileName)
{
	// Load map
	this->map = this->mapReader->loadMap(fileName);

	// TODO enable this if needed
	// Edge case where accidental small rectangles are leftover, clean these up
	//SKO_MapEditor::Manager::cleanupInvisibleRects(this->map);
	// Remove tiles that are the same tileID and same X and same Y values
	//removeDuplicateTiles(map);
}

void SKO_MapEditor::Manager::DrawGameScene(int camera_x, int camera_y)
{
	//image buffer and background
	renderer->drawImage(0, 0, &background);
	renderer->drawImage(1024, 0, &background);
	renderer->drawImage(0, 1024, &background);
	renderer->drawImage(1024, 1024, &background);

	// TODO use sprite batching
	//draw background tiles, only on screen
	//
	//{
	//	auto tilesetKey = it->first;
	//	auto tiles = it->second;

	//	for (auto tile : tiles)
	//	{
	//		tile->image = tilesets[tilesetKey]->getTileImage(tile->tileset_row, tile->tileset_column);
	//	}
	//}

	for (auto it = map->backgroundTiles.begin(); it != map->backgroundTiles.end(); it++)
	{
		auto tilesetKey = it->first;
		auto tileset = tilesets[tilesetKey];
		auto tiles = it->second;

		for (auto tile : tiles)
		{
			int drawX = tile->x - camera_x;
			int drawY = tile->y - camera_y;

			SDL_Rect selection;
			selection.x = tile->tileset_column * tileset->tile_width;
			selection.y = tile->tileset_row * tileset->tile_height;
			selection.w = tileset->tile_width;
			selection.h = tileset->tile_height;

			//TODO only bind image once 
			renderer->drawImage(drawX, drawY, tileset->image, selection);
		}
	}

	//stickman!
	if (stickman_toggle) //TODO - mapEditorState.stickmanVisible
		renderer->drawImage(stickman.x - 25 - camera_x, stickman.y - camera_y, &stickman_img);


	//draw fringe tiles, only on screen
	for (auto it = map->fringeTiles.begin(); it != map->fringeTiles.end(); it++)
	{
		auto tilesetKey = it->first;
		auto tileset = tilesets[tilesetKey];
		auto tiles = it->second;

		for (auto tile : tiles)
		{
			int drawX = tile->x - camera_x;
			int drawY = tile->y - camera_y;

			SDL_Rect selection;
			selection.x = tile->tileset_column * tileset->tile_width;
			selection.y = tile->tileset_row * tileset->tile_height;
			selection.w = tileset->tile_width;
			selection.h = tileset->tile_height;

			//TODO only bind image once 
			renderer->drawImage(drawX, drawY, tileset->image, selection);
		}
	}

	//draw current tile
	if (mode == TILE_DRAW) {


		int drawX = cursor_x / 32 * 32;
		int drawY = cursor_y / 32 * 32;

		SDL_Rect selection;
		selection.x = current_tileset_column * current_tileset->tile_width;
		selection.y = current_tileset_row * current_tileset->tile_height;
		selection.w = current_tileset->tile_width;
		selection.h = current_tileset->tile_height;

		renderer->drawImage(drawX, drawY, current_tileset->image, selection);
	}

	//draw collision rects, only on screen
	for (int i = 0; i < map->collisionRects.size(); i++)
	{
		SDL_Rect newRect;

		newRect.x = map->collisionRects[i].x - (int)camera_x;
		newRect.y = map->collisionRects[i].y - (int)camera_y;
		newRect.h = map->collisionRects[i].h;
		newRect.w = map->collisionRects[i].w;

		if (newRect.x >= 0 - newRect.w && 
			newRect.x < renderer->originalWindowWidth && 
			newRect.y < renderer->originalWindowHeight &&
			newRect.y >= 0 - newRect.h)
			renderer->drawRect(newRect, 0, 200, 200);
	}
}

void SKO_MapEditor::Manager::DrawGui()
{
	for (OPI_Gui::Element* child : gui->children) 
	{
		if (child->isVisible)
			this->gui->drawElement(0, 0, child);
	}
}


void SKO_MapEditor::Manager::HandleInput()
{
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_WINDOWEVENT)
		{
			switch (event.window.event)
			{
			case SDL_WINDOWEVENT_SHOWN:
				SDL_Log("Window %d shown", event.window.windowID);
				break;
			case SDL_WINDOWEVENT_HIDDEN:
				SDL_Log("Window %d hidden", event.window.windowID);
				break;
			case SDL_WINDOWEVENT_EXPOSED:
				SDL_Log("Window %d exposed", event.window.windowID);
				break;
			case SDL_WINDOWEVENT_MOVED:
				SDL_Log("Window %d moved to %d,%d",
					event.window.windowID, event.window.data1,
					event.window.data2);
				break;
			case SDL_WINDOWEVENT_RESIZED:
				SDL_Log("Window %d resized to %dx%d",
					event.window.windowID, event.window.data1,
					event.window.data2);

				renderer->windowWidth = event.window.data1;
				renderer->windowHeight = event.window.data2;
				renderer->sizeScreen();

				break;
			}
		}

		if (event.type == SDL_QUIT)
		{
			//todo save a backup file
			done = true;
		}

		if (event.type == SDL_KEYUP)
		{
			switch (event.key.keysym.sym)
			{
			case 'a':
				LEFT = false;
				break;

			case 'd':
				RIGHT = false;
				break;

			case SDLK_LSHIFT:
				SHIFT_HELD = false;
				break;

			default:
				break;
			}
		}

		if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_LSHIFT:
				SHIFT_HELD = true;
				break;
			case '1':
				mode = TILE_DRAW;
				break;
			case '2':
				mode = TILE_DELETE;
				break;
			case '3':
				fringe_mode = !fringe_mode;
				break;
			case '4':
				mode = COLLISION_DRAW;
				break;
			case '5':
				mode = COLLISION_DELETE;
				break;
			case '6':
				mode = TOGGLE_TEST;
				break;
			case '7':
				stickman_toggle = !stickman_toggle;
				break;
			case '8':
			{
				saveMap();
			}
			break;


			case  SDLK_UP:
				if (SHIFT_HELD)
					camera_y -= 576;
				else
					camera_y -= 32;
				break;
			case  SDLK_LEFT:
				if (SHIFT_HELD)
					camera_x -= 1024;
				else
					camera_x -= 32;
				break;
			case  SDLK_DOWN:
				if (SHIFT_HELD)
					camera_y += 576;
				else
					camera_y += 32;
				break;
			case  SDLK_RIGHT:
				if (SHIFT_HELD)
					camera_x += 1024;
				else
					camera_x += 32;
				break;


			case SDLK_HOME:
				camera_x = 0;
				camera_y = 0;
				break;
			case SDLK_PAGEUP: // TODO: next / prev tile function
			{
				if (current_tileset_column == 0)
				{
					current_tileset_column = current_tileset->columns - 1;
					
					if (current_tileset_row == 0)
					{
						current_tileset_row = current_tileset->rows - 1;
					}
					else 
					{
						current_tileset_row--;
					}
				}
				else
				{
					current_tileset_column--;
				}

			}break;

			case SDLK_PAGEDOWN: // TODO: next / prev tile function
			{
				current_tileset_column++;

				if (current_tileset_column >= current_tileset->columns)
				{
					current_tileset_column = 0;
					current_tileset_row++;

					if (current_tileset_row >= current_tileset->rows)
					{
						current_tileset_row = 0;
					}
				}
			}break;

			case 'w':
				if (current_tile != nullptr && mode == TILE_DRAW)
					current_tile->y--;

				if (mode == TOGGLE_TEST)
					y_speed = -6;

				if (mode == COLLISION_DRAW && current_rect >= 0)
				{
					if (map->collisionRects[current_rect].y > 0)
					{
						map->collisionRects[current_rect].y--;
						map->collisionRects[current_rect].h++;
					}
				}
				break;
			case 'a':

				if (current_tile > 0 && mode == TILE_DRAW && !fringe_mode)
				{
					current_tile->x--;
				}

				LEFT = true;

				if (mode == COLLISION_DRAW && current_rect >= 0)
				{
					if (map->collisionRects[current_rect].x > 0)
					{
						map->collisionRects[current_rect].x--;
						map->collisionRects[current_rect].w++;
					}
				}
				break;
			case 's':
				if (current_tile != nullptr && mode == TILE_DRAW)
					current_tile->y++;
				
				if (mode == COLLISION_DRAW && current_rect >= 0)
				{
					map->collisionRects[current_rect].y++;
					map->collisionRects[current_rect].h--;
				}

				break;
			case 'd':

				if (current_tile != nullptr && mode == TILE_DRAW)
					current_tile->x++;

				RIGHT = true;

				if (mode == COLLISION_DRAW && current_rect >= 0)
				{
					map->collisionRects[current_rect].x++;
					map->collisionRects[current_rect].w--;
				}
				break;

				//smaller rects
			case 'i':
				if (mode == COLLISION_DRAW && current_rect >= 0)
				{
					if (map->collisionRects[current_rect].h > 0)
					{
						map->collisionRects[current_rect].h--;
					}
				}
				break;
			case 'j':
				if (mode == COLLISION_DRAW && current_rect >= 0)
				{
					if (map->collisionRects[current_rect].w > 0)
					{
						map->collisionRects[current_rect].w--;
					}
				}
				break;
			case 'k':
				if (mode == COLLISION_DRAW && current_rect >= 0)
				{
					map->collisionRects[current_rect].h++;
				}
				break;
			case 'l':
				if (mode == COLLISION_DRAW && current_rect >= 0)
				{
					map->collisionRects[current_rect].w++;
				}
				break;


			default:
				printf("unknown key is: %i\n", (int)event.key.keysym.sym);
				break;

			}
		}

		//reset camera
		if (camera_x < 0)
			camera_x = 0;
		if (camera_y < 0)
			camera_y = 0;

		if (event.type == SDL_MOUSEMOTION)
		{
			//Get the mouse offsets, scaled from screen space to window space
			cursor_x = renderer->getScaledMouseX(event.motion.x);
			cursor_y = renderer->getScaledMouseY(event.motion.y);

			bool eventHandled = gui->handleMouseMove(cursor_x, cursor_y);

			if (eventHandled)
				break;

			//draw the almost done rect
			if (mode == COLLISION_DRAW && current_rect >= 0 && LCLICK)
			{
				//variables for easy determining
				int cx = cursor_x + (int)camera_x;
				int cy = cursor_y + (int)camera_y;
				int x1, x2, y1, y2;

				//
				// X
				//
				if (cx > collision_ox)
				{
					x1 = map->collisionRects[current_rect].x;
					x2 = cx;
				}
				else
				{
					x1 = cx;
					x2 = collision_ox;
				}

				//
				// Y
				//
				if (cy > collision_oy)
				{
					y1 = map->collisionRects[current_rect].y;
					y2 = cy;
				}
				else
				{
					y1 = cy;
					y2 = collision_oy;
				}

				//adjust the width and height
				map->collisionRects[current_rect].x = x1;
				map->collisionRects[current_rect].w = x2 - x1;
				map->collisionRects[current_rect].y = y1;
				map->collisionRects[current_rect].h = y2 - y1;
			}
			if (mode == TILE_DRAW && LCLICK)
			{
				current_tile->x = (int)(cursor_x + camera_x) / 32 * 32;
				current_tile->y = (int)(cursor_y + camera_y) / 32 * 32;
			}
			if (mode == TOGGLE_TEST && LCLICK)
			{
				stickman.x = cursor_x + camera_x;
				stickman.y = cursor_y + camera_y;
			}
		}

		if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && !LCLICK)
		{
			bool eventHandled = gui->handleMousePressLeft(cursor_x, cursor_y);
			LCLICK = true;

			if (eventHandled)
				break;

			switch (mode)
			{

			case TILE_DRAW: {
				if (!placing_tile)
				{
					placing_tile = true;
				}

				auto x = (int)(cursor_x + camera_x) / 32 * 32;
				auto y = (int)(cursor_y + camera_y) / 32 * 32;
				auto tile = new SKO_Map::Tile(x, y, current_tileset, current_tileset_row, current_tileset_column);
				if (fringe_mode)
				{
					map->fringeTiles[current_tileset->key].push_back(tile);
				}
				else
				{
					map->backgroundTiles[current_tileset->key].push_back(tile);
				}
				current_tile = tile;
			}
			break;

			case TILE_DELETE: {
				//find a tile to delete
				int x = cursor_x + camera_x;
				int y = cursor_y + camera_y;
				int width = current_tileset->tile_width;
				int height = current_tileset->tile_height;
				std::vector<SKO_Map::Tile*> *tiles; 

				if (fringe_mode)
				{
					tiles = &map->fringeTiles[current_tileset->key];
				}
				else
				{
					tiles = &map->backgroundTiles[current_tileset->key];
				}

				int at = -1;
				for (int i = 0; i < tiles->size(); i++)
				{
					auto tile = (*tiles)[i];
					if (x > tile->x && x < tile->x + width &&
						y > tile->y && y < tile->y + height)
					{
						at = i;
						break;
					}
				}
					
				if (at >= 0)
				{
					tiles->erase(tiles->begin() + at);
				}
			}
			break;

			// TODO separate all these giant switch statements please
			case COLLISION_DRAW: {

				current_rect++;

				//Get the mouse offsets, scaled from screen space to window space
				cursor_x = renderer->getScaledMouseX(event.motion.x);
				cursor_y = renderer->getScaledMouseY(event.motion.y);

				//Get the mouse offsets
				collision_ox = cursor_x + (int)camera_x;
				collision_oy = cursor_y + (int)camera_y;

				SDL_Rect newRect;
				newRect.x = collision_ox;
				newRect.y = collision_oy;
				newRect.w = 0;
				newRect.h = 0;
				map->collisionRects.push_back(newRect);
			}break;
			case COLLISION_DELETE: {
				//find a collision to delete
				int i;
				int x = cursor_x + camera_x;
				int y = cursor_y + camera_y;
				int at = -1;
				for (i = 0; i <= current_rect; i++)
				{
					if (x > map->collisionRects[i].x && x < map->collisionRects[i].x + map->collisionRects[i].w &&
						y > map->collisionRects[i].y && y < map->collisionRects[i].y + map->collisionRects[i].h)
					{
						at = i;
						break;
					}
				}

				if (at >= 0)
				{
					map->collisionRects.erase(map->collisionRects.begin() + at);
					current_rect--;
				}
			}
			break;


			case TOGGLE_TEST:
				stickman_toggle = true;
				stickman.x = cursor_x + camera_x;
				stickman.y = cursor_y + camera_y;

				break;


			}

		}

		if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_RIGHT && !RCLICK)
		{
			bool eventHandled = gui->handleMousePressRight(cursor_x, cursor_y);
			RCLICK = true;
			if (eventHandled)
				break;


			//TODO - use RCLICK for collision rect editing
		}
		if (event.type == SDL_MOUSEBUTTONUP)
		{
			if (event.button.button == SDL_BUTTON_LEFT && LCLICK)
			{
				bool eventHandled = gui->handleMouseReleaseLeft(cursor_x, cursor_y);
				LCLICK = false;

				if (eventHandled)
					break;

				switch (mode)
				{
				case TILE_DRAW:
					if (!fringe_mode)
					{
						placing_tile = false;
					}
					else
					{
						placing_fringe = false;
					}

					break;

				case COLLISION_DRAW:
					// Cleanup small rects you can't see
					cleanupInvisibleRects(map);
					break;
				}


			}

			if (event.button.button == SDL_BUTTON_RIGHT && RCLICK)
			{
				bool eventHandled = gui->handleMouseReleaseRight(cursor_x, cursor_y);
				RCLICK = false;

				if (eventHandled)
					break;


			}
		}
	} //if poll event
}

void SKO_MapEditor::Manager::processLoop()
{
	//if save notify is in the past, clear it
	if (save_notify < clock())
		save_notify = 0;

	if (OPI_Clock::milliseconds() - coordsTicker > 15)
	{
		//draw coords
		std::stringstream ss;
		ss << "(" << cursor_x + camera_x << ", " << cursor_y + camera_y << ")";

		mainMenuGui->setCoords(ss.str());

		//reset ticker
		coordsTicker = OPI_Clock::milliseconds();
	}

	graphics();
	HandleInput();

	if (!done && stickman_toggle)
		physics();

	//prevent repeats
	this->event.type = 0;
}

// Simulate Physics of test stickman

bool SKO_MapEditor::Manager::isBlocked(SDL_Rect rect)
{
	for (int i = 0; i < this->map->collisionRects.size(); i++)
	{
		if (SDL_HasIntersection(&rect, &this->map->collisionRects[i]))
			return true;
	}

	return false;
}

void SKO_MapEditor::Manager::graphics()
{
	renderer->startDraw();

	// Draw map, tiles, etc
	DrawGameScene((int)this->camera_x, (int)this->camera_y);

	// Draw Map Editor Gui
	DrawGui();

	//update screen
	renderer->updateScreen();
}

void SKO_MapEditor::Manager::physics()
{
	// fall
	if (y_speed < 10)
	{
		y_speed += GRAVITY;
	}

	//verical collision detection
	SDL_Rect stickmanRect;
	stickmanRect.x = stickman.x;
	stickmanRect.y = stickman.y + y_speed;
	stickmanRect.w = 14;
	stickmanRect.h = 63;
	bool block_y = isBlocked(stickmanRect);

	//vertical movement
	if (!block_y)
	{
		//not blocked, fall
		stickman.y += y_speed;
	}
	else
	{
		y_speed = 0;
	}

	//horizontal collision detection
	stickmanRect.x = stickman.x + x_speed;
	stickmanRect.y = stickman.y;
	bool block_x = isBlocked(stickmanRect);

	//horizontal movement
	if (!block_x)
	{
		//not blocked, walk
		stickman.x += (x_speed);
	}

	if (LEFT && x_speed != -2)
	{
		x_speed = -2;
	}
	if (RIGHT && x_speed != 2)
	{
		x_speed = 2;
	}

	if (LEFT == RIGHT)
	{
		if (x_speed != 0)
			x_speed = 0;
	}
}