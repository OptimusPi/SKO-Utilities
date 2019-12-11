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

	// Load tiles and images
	background.setImage("IMG/back.png");
	stickman_img.setImage("IMG/stickman.png");

	// Load tileset
	for (int i = 0; i < 256; i++)//check if file exists, etc.
	{
		std::stringstream ss;
		ss << "IMG/TILE/tile" << i << ".png";
		std::ifstream checker(ss.str());
		if (checker.is_open())
		{
			checker.close();
			tile_img[i].setImage(ss.str());
			num_tile_images = i;
		}
		else
			break;
	}
}

SKO_MapEditor::Manager::~Manager()
{

}
 
// Clean up useless rectangles (<4x4 pixels) to a SKO_Map in-place.
void SKO_MapEditor::Manager::cleanupInvisibleRects(SKO_Map::Map *map)
{
	std::vector<int> invisibleRectIds;

	//Delete any collision rectangles that are too small.
	for (int i = 0; i < map->collisionRects.size(); i++)
	{
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

			if (tileA->tileId == tileB->tileId && tileA->x == tileB->x && tileA->y == tileB->y)
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
	removeDuplicateTiles(&map->backgroundTiles);
	removeDuplicateTiles(&map->fringeTiles);
}

 
void SKO_MapEditor::Manager::saveMap(std::string fileLocation)
{
	// Cleanup small rects you can't see
	this->cleanupInvisibleRects(this->map);

	// Save the map at the given file location
	this->map->saveMap(fileLocation);
}

void SKO_MapEditor::Manager::saveMap()
{
	// Cleanup small rects you can't see
	this->cleanupInvisibleRects(this->map);

	// Save the map to its previous file location
	this->map->saveMap();
}


void SKO_MapEditor::Manager::loadMap(std::string fileName)
{
	// Load map
	this->map = this->mapReader->loadMap(fileName);

	// Edge case where accidental small rectangles are leftover, clean these up
	//SKO_MapEditor::Manager::cleanupInvisibleRects(this->map);
	// Remove tiles that are the same tileID and same X and same Y values
	//removeDuplicateTiles(map);

	// Set map current rect
	current_rect = map->collisionRects.size() - 1;
	current_fringe = map->fringeTiles.size() - 1;
	current_tile = map->backgroundTiles.size() - 1;
}

void SKO_MapEditor::Manager::DrawGameScene(int camera_x, int camera_y)
{
	//image buffer and background
	renderer->drawImage(0, 0, &background);
	renderer->drawImage(1024, 0, &background);
	renderer->drawImage(0, 1024, &background);
	renderer->drawImage(1024, 1024, &background);

	//draw background tiles, only on screen
	for (int i = 0; i < map->backgroundTiles.size(); i++)
	{
		SKO_Map::Tile *tile = map->backgroundTiles[i];
		OPI_Image *tileImg = &tile_img[tile->tileId];

		int drawX = tile->x - camera_x;
		int drawY = tile->y - camera_y;

		if (drawX >= (int)(0 - tileImg->width) &&
			drawX < renderer->originalWindowWidth &&
			drawY < renderer->originalWindowHeight &&
			drawY >= (int)(0 - tileImg->height))
		{
			renderer->drawImage(drawX, drawY, tileImg);
		}
	}

	//stickman!
	if (stickman_toggle) //TODO - mapEditorState.stickmanVisible
		renderer->drawImage(stickman.x - 25 - camera_x, stickman.y - camera_y, &stickman_img);


	//draw finge tiles, only on screen
	for (int i = 0; i < map->fringeTiles.size(); i++)
	{
		SKO_Map::Tile *tile = map->fringeTiles[i];
		OPI_Image *tileImg = &tile_img[tile->tileId];


		int drawX = tile->x - camera_x;
		int drawY = tile->y - camera_y;

		if (drawX >= (int)(0 - tileImg->width) &&
			drawX < renderer->originalWindowWidth &&
			drawY < renderer->originalWindowHeight &&
			drawY >= (int)(0 - tileImg->height))
		{
			renderer->drawImage(drawX, drawY, tileImg);
		}
		
	}

	

	//draw current tile
	if (mode == TILE_DRAW)
		renderer->drawImage(cursor_x / 32 * 32, cursor_y / 32 * 32, &tile_img[current_tile_img]);

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
			case SDLK_PAGEUP:
				if (current_tile_img > 0)
					current_tile_img--;
				break;

			case SDLK_PAGEDOWN:
				if (current_tile_img < num_tile_images)
					current_tile_img++;
				break;

			case 'w':
				if (current_tile > 0 && mode == TILE_DRAW && !fringe_mode)
					map->backgroundTiles[current_tile - 1]->x--;
				if (current_fringe > 0 && mode == TILE_DRAW && fringe_mode)
					map->fringeTiles[current_fringe - 1]->x--;

				if (mode == TOGGLE_TEST)
					y_speed = -6;
				if (mode == COLLISION_DRAW && current_rect > 0)
				{
					if (map->collisionRects[current_rect - 1].y > 0)
					{
						map->collisionRects[current_rect - 1].y--;
						map->collisionRects[current_rect - 1].w++;
					}
				}
				break;
			case 'a':

				if (current_tile > 0 && mode == TILE_DRAW && !fringe_mode)
				{
					map->backgroundTiles[current_tile - 1]->x--;
				}
				if (current_fringe > 0 && mode == TILE_DRAW && fringe_mode)
				{
					map->fringeTiles[current_fringe - 1]->x--;
				}

				LEFT = true;

				if (mode == COLLISION_DRAW && current_rect > 0)
				{
					if (map->collisionRects[current_rect - 1].x > 0)
					{
						map->collisionRects[current_rect - 1].x--;
						map->collisionRects[current_rect - 1].w++;
					}
				}
				break;
			case 's':
				if (current_tile > 0 && mode == TILE_DRAW && !fringe_mode)
					map->backgroundTiles[current_tile - 1]->y++;
				if (current_fringe > 0 && mode == TILE_DRAW && fringe_mode)
					map->fringeTiles[current_fringe - 1]->y++;

				if (mode == COLLISION_DRAW && current_rect > 0)
				{
					map->collisionRects[current_rect - 1].y++;
					map->collisionRects[current_rect - 1].h--;
				}

				break;
			case 'd':

				if (current_tile > 0 && mode == TILE_DRAW && !fringe_mode)
					map->backgroundTiles[current_tile - 1]->x++;
				if (current_fringe > 0 && mode == TILE_DRAW && fringe_mode)
					map->fringeTiles[current_fringe - 1]->x++;

				RIGHT = true;

				if (mode == COLLISION_DRAW && current_rect > 0)
				{
					map->collisionRects[current_rect - 1].x++;
					map->collisionRects[current_rect - 1].w--;
				}
				break;

				//smaller rects
			case 'i':
				if (mode == COLLISION_DRAW && current_rect > 0)
				{
					if (map->collisionRects[current_rect - 1].h > 0)
					{
						map->collisionRects[current_rect - 1].h--;
					}
				}
				break;
			case 'j':
				if (mode == COLLISION_DRAW && current_rect > 0)
				{
					if (map->collisionRects[current_rect - 1].w > 0)
					{
						map->collisionRects[current_rect - 1].w--;
					}
				}
				break;
			case 'k':
				if (mode == COLLISION_DRAW && current_rect > 0)
				{
					map->collisionRects[current_rect - 1].h++;
				}
				break;
			case 'l':
				if (mode == COLLISION_DRAW && current_rect > 0)
				{
					map->collisionRects[current_rect - 1].w++;
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
			if (mode == COLLISION_DRAW && LCLICK)
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
				if (!fringe_mode)
				{
					map->backgroundTiles[current_tile]->x = (int)(cursor_x + camera_x) / 32 * 32;
					map->backgroundTiles[current_tile]->y = (int)(cursor_y + camera_y) / 32 * 32;
				}
				else
				{
					map->fringeTiles[current_fringe]->x = (int)(cursor_x + camera_x) / 32 * 32;
					map->fringeTiles[current_fringe]->y = (int)(cursor_y + camera_y) / 32 * 32;
				}
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

			case TILE_DRAW:
				if (!fringe_mode)
				{
					if (!placing_tile)
					{
						placing_tile = true;
					}

					int x = (int)(cursor_x + (int)camera_x) / 32 * 32;
					int y = (int)(cursor_y + (int)camera_y) / 32 * 32;

					map->backgroundTiles.push_back(new SKO_Map::Tile(x, y, current_tile_img));
				}
				else
				{
					if (!placing_fringe)
					{
						placing_fringe = true;
					}

					int x = (int)(cursor_x + (int)camera_x) / 32 * 32;
					int y = (int)(cursor_y + (int)camera_y) / 32 * 32;
					map->fringeTiles.push_back(new SKO_Map::Tile(x, y, current_tile_img));
				}
				break;

			case TILE_DELETE: {
				//find a tile to delete
				int i;
				int x = cursor_x + camera_x;
				int y = cursor_y + camera_y;

				if (!fringe_mode)
				{
					int at = -1;
					for (i = 0; i < current_tile; i++)
					{
						if (x > map->backgroundTiles[i]->x && x < map->backgroundTiles[i]->x + tile_img[map->backgroundTiles[i]->tileId].width &&
							y > map->backgroundTiles[i]->y && y < map->backgroundTiles[i]->y + tile_img[map->backgroundTiles[i]->tileId].height)
						{
							at = i;
							break;
						}
					}

					if (at >= 0) 
					{
						map->backgroundTiles.erase(map->backgroundTiles.begin() + at);
						current_tile--;
					}
				}
				else
				{
					int at = -1;
					for (i = 0; i < current_fringe; i++)
					{
						if (x > map->fringeTiles[i]->x && x < map->fringeTiles[i]->x + tile_img[map->fringeTiles[i]->tileId].width &&
							y > map->fringeTiles[i]->y && y < map->fringeTiles[i]->y + tile_img[map->fringeTiles[i]->tileId].height)
						{
							at = i;
							break;
						}
					}

					if (at >= 0)
					{
						map->fringeTiles.erase(map->fringeTiles.begin() + at);
						current_fringe--;
					}
				}
			}
			break;

			case COLLISION_DRAW:

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
				break;

			case COLLISION_DELETE: {
				//find a collision to delete
				int i;
				int x = cursor_x + camera_x;
				int y = cursor_y + camera_y;
				int at = -1;
				for (i = 0; i < current_rect; i++)
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
						//go to the next tile
						current_tile++;

						map->backgroundTiles[current_tile]->x = (int)(cursor_x + (int)camera_x) / 32 * 32;
						map->backgroundTiles[current_tile]->y = (int)(cursor_y + (int)camera_y) / 32 * 32;
						map->backgroundTiles[current_tile]->tileId = current_tile_img;

						
						placing_tile = false;
					}
					else
					{

						current_fringe++;
						map->fringeTiles[current_fringe]->x = (int)(cursor_x + (int)camera_x) / 32 * 32;
						map->fringeTiles[current_fringe]->y = (int)(cursor_y + (int)camera_y) / 32 * 32;
						map->fringeTiles[current_fringe]->tileId = current_tile_img;

						//go to the next tile
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