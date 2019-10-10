#include "MapEditorManager.h"

SKO_MapEditor::Manager::Manager(OPI_Renderer * renderer)
{
	this->renderer = renderer;
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
		if (map->collisionRects[i]->h < 4 || map->collisionRects[i]->w < 4)
		{
			current_rect--;
			invisibleRectIds.push_back(i);
		}
	}

	for (int i = 0; i < invisibleRectIds.size(); i++)
	{
		map->collisionRects.erase(map->collisionRects.begin() + invisibleRectIds[i]);
	}
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
	this->map = new SKO_Map::Map(fileName);

	// Edge case where accidental small rectangles are leftover, clean these up
	SKO_MapEditor::Manager::cleanupInvisibleRects(this->map);
}

void SKO_MapEditor::Manager::DrawGameScene()
{
	//image buffer and background
	renderer->drawImage(0, 0, &background);
	renderer->drawImage(1024, 0, &background);
	renderer->drawImage(0, 1024, &background);
	renderer->drawImage(1024, 1024, &background);

	//draw background tiles, only on screen
	for (int i = 0; i < map->backGroundTiles; i++)
	{   
		OPI_Map::Tile *tile = mapEditorManager->getBackgroundTile(i);
		int drawX = map->tile[i] - (int)camera_x;
		int drawY = map->tile[i] - (int)camera_y;


		if (drawX >= (int)(0 - tile_img[tile[i]].width) &&
			drawX < renderer->originalWindowWidth &&
			drawY < renderer->originalWindowHeight &&
			drawY >= (int)(0 - tile_img[tile[i]].height))
			renderer->drawImage(drawX, drawY, tile);
	}

	//stickman!
	if (stickman_toggle) //TODO - mapEditorState.stickmanVisible
		renderer->drawImage(stickman.x - 25 - camera_x, stickman.y - camera_y, &stickman_img);

	//draw tiles, only on screen
	for (int i = 0; i < map->number_of_fringe; i++)
	{
		int draw_x = fringe_x[i] - (int)camera_x;
		int draw_y = fringe_y[i] - (int)camera_y;

		if (draw_x >= 0 - tile_img[fringe[i]].width &&
			draw_x < renderer->originalWindowWidth && draw_y < renderer->originalWindowHeight &&
			draw_y >= 0 - tile_img[fringe[i]].height)
			renderer->drawImage(draw_x, draw_y, &tile_img[fringe[i]]);
	}

	//draw current tile
	if (mode == TILE_DRAW)
		renderer->drawImage(cursor_x / 32 * 32, cursor_y / 32 * 32, &tile_img[current_tile_img]);

	//draw collision rects, only on screen
	for (int i = 0; i < number_of_rects; i++)
	{
		SDL_Rect newRect;

		newRect.x = collision_rect[i].x - (int)camera_x;
		newRect.y = collision_rect[i].y - (int)camera_y;
		newRect.h = collision_rect[i].h;
		newRect.w = collision_rect[i].w;

		if (newRect.x >= 0 - collision_rect[i].w &&
			newRect.x < renderer->originalWindowWidth && newRect.y < renderer->originalWindowHeight &&
			newRect.y >= 0 - collision_rect[i].h)
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


void HandleInput(SDL_Event event)
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
				if (current_tile_img < num_tiles)
					current_tile_img++;
				break;

			case 'w':
				if (current_tile > 0 && mode == TILE_DRAW && !fringe_mode)
					tile_y[current_tile - 1]--;
				if (current_fringe > 0 && mode == TILE_DRAW && fringe_mode)
					fringe_y[current_fringe - 1]--;

				if (mode == TOGGLE_TEST)
					y_speed = -6;
				if (mode == COLLISION_DRAW && current_rect > 0)
				{
					if (collision_rect[current_rect - 1].y > 0)
					{
						collision_rect[current_rect - 1].y--;
						collision_rect[current_rect - 1].w++;
					}
				}
				break;
			case 'a':

				if (current_tile > 0 && mode == TILE_DRAW && !fringe_mode)
				{
					tile_x[current_tile - 1]--;
				}
				if (current_fringe > 0 && mode == TILE_DRAW && fringe_mode)
				{
					fringe_x[current_fringe - 1]--;
				}

				LEFT = true;

				if (mode == COLLISION_DRAW && current_rect > 0)
				{
					if (collision_rect[current_rect - 1].x > 0)
					{
						collision_rect[current_rect - 1].x--;
						collision_rect[current_rect - 1].w++;
					}
				}
				break;
			case 's':
				if (current_tile > 0 && mode == TILE_DRAW && !fringe_mode)
					tile_y[current_tile - 1]++;
				if (current_fringe > 0 && mode == TILE_DRAW && fringe_mode)
					fringe_y[current_fringe - 1]++;

				if (mode == COLLISION_DRAW && current_rect > 0)
				{
					collision_rect[current_rect - 1].y++;
					collision_rect[current_rect - 1].h--;
				}

				break;
			case 'd':

				if (current_tile > 0 && mode == TILE_DRAW && !fringe_mode)
					tile_x[current_tile - 1]++;
				if (current_fringe > 0 && mode == TILE_DRAW && fringe_mode)
					fringe_x[current_fringe - 1]++;

				RIGHT = true;

				if (mode == COLLISION_DRAW && current_rect > 0)
				{
					collision_rect[current_rect - 1].x++;
					collision_rect[current_rect - 1].w--;
				}
				break;

				//smaller rects
			case 'i':
				if (mode == COLLISION_DRAW && current_rect > 0)
				{
					if (collision_rect[current_rect - 1].h > 0)
					{
						collision_rect[current_rect - 1].h--;
					}
				}
				break;
			case 'j':
				if (mode == COLLISION_DRAW && current_rect > 0)
				{
					if (collision_rect[current_rect - 1].w > 0)
					{
						collision_rect[current_rect - 1].w--;
					}
				}
				break;
			case 'k':
				if (mode == COLLISION_DRAW && current_rect > 0)
				{
					collision_rect[current_rect - 1].h++;
				}
				break;
			case 'l':
				if (mode == COLLISION_DRAW && current_rect > 0)
				{
					collision_rect[current_rect - 1].w++;
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
					x1 = collision_rect[current_rect].x;
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
					y1 = collision_rect[current_rect].y;
					y2 = cy;
				}
				else
				{
					y1 = cy;
					y2 = collision_oy;
				}

				//adjust the width and height
				collision_rect[current_rect].x = x1;
				collision_rect[current_rect].w = x2 - x1;
				collision_rect[current_rect].y = y1;
				collision_rect[current_rect].h = y2 - y1;
			}
			if (mode == TILE_DRAW && LCLICK)
			{
				if (!fringe_mode)
				{
					tile_x[current_tile] = (int)(cursor_x + camera_x) / 32 * 32;
					tile_y[current_tile] = (int)(cursor_y + camera_y) / 32 * 32;
				}
				else
				{
					fringe_x[current_fringe] = (int)(cursor_x + camera_x) / 32 * 32;
					fringe_y[current_fringe] = (int)(cursor_y + camera_y) / 32 * 32;
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
						number_of_tiles++;
						placing_tile = true;
					}

					tile_x[current_tile] = (int)(cursor_x + (int)camera_x) / 32 * 32;
					tile_y[current_tile] = (int)(cursor_y + (int)camera_y) / 32 * 32;
					tile[current_tile] = current_tile_img;
				}
				else
				{
					if (!placing_fringe)
					{
						number_of_fringe++;
						placing_fringe = true;
					}

					fringe_x[current_fringe] = (int)(cursor_x + (int)camera_x) / 32 * 32;
					fringe_y[current_fringe] = (int)(cursor_y + (int)camera_y) / 32 * 32;
					fringe[current_fringe] = current_tile_img;
				}
				break;

			case TILE_DELETE: {
				//find a tile to delete
				int i;
				int x = cursor_x + camera_x;
				int y = cursor_y + camera_y;

				if (!fringe_mode)
				{
					bool found = false;
					for (i = 0; i < current_tile; i++)
					{
						if (x > tile_x[i] && x < tile_x[i] + tile_img[tile[i]].width &&
							y > tile_y[i] && y < tile_y[i] + tile_img[tile[i]].height)
						{
							found = true;
							break;
						}
					}

					//move all tiles back in the array
					for (; found && i < current_tile; i++)
					{
						tile[i] = tile[i + 1];
						tile_x[i] = tile_x[i + 1];
						tile_y[i] = tile_y[i + 1];
					}

					//we deleted a tile. current position is one less now.
					if (found) {
						current_tile--;
						number_of_tiles--;
					}
				}
				else
				{
					bool found = false;
					for (i = 0; i < current_fringe; i++)
					{
						if (x > fringe_x[i] && x < fringe_x[i] + tile_img[fringe[i]].width &&
							y > fringe_y[i] && y < fringe_y[i] + tile_img[fringe[i]].height)
						{
							found = true;
							break;
						}
					}

					//move all tiles back in the array
					for (; found && i < current_fringe; i++)
					{
						fringe[i] = fringe[i + 1];
						fringe_x[i] = fringe_x[i + 1];
						fringe_y[i] = fringe_y[i + 1];
					}

					//we deleted a tile. current position is one less now.
					if (found) {
						current_fringe--;
						number_of_fringe--;
					}
				}
			}
							  break;

			case COLLISION_DRAW:
				//go to next rect
				number_of_rects++;

				//Get the mouse offsets, scaled from screen space to window space
				cursor_x = renderer->getScaledMouseX(event.motion.x);
				cursor_y = renderer->getScaledMouseY(event.motion.y);

				//Get the mouse offsets
				collision_ox = cursor_x + (int)camera_x;
				collision_oy = cursor_y + (int)camera_y;
				collision_rect[current_rect].x = collision_ox;
				collision_rect[current_rect].y = collision_oy;
				collision_rect[current_rect].w = 0;
				collision_rect[current_rect].h = 0;
				break;

			case COLLISION_DELETE: {
				//find a collision to delete
				int i;
				int x = cursor_x + camera_x;
				int y = cursor_y + camera_y;
				bool found = false;
				for (i = 0; i < current_rect; i++)
				{
					if (x > collision_rect[i].x && x < collision_rect[i].x + collision_rect[i].w &&
						y > collision_rect[i].y && y < collision_rect[i].y + collision_rect[i].h)
					{
						found = true;
						break;
					}
				}

				//move all tiles back in the array
				for (; i < current_rect; i++)
				{
					collision_rect[i] = collision_rect[i + 1];
				}


				//we deleted a rect. current position is one less now.
				if (found) {
					current_rect--;
					number_of_rects--;
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
						tile_x[current_tile] = (int)(cursor_x + (int)camera_x) / 32 * 32;
						tile_y[current_tile] = (int)(cursor_y + (int)camera_y) / 32 * 32;
						tile[current_tile] = current_tile_img;

						//go to the next tile
						current_tile++;
						placing_tile = false;
					}
					else
					{
						fringe_x[current_fringe] = (int)(cursor_x + (int)camera_x) / 32 * 32;
						fringe_y[current_fringe] = (int)(cursor_y + (int)camera_y) / 32 * 32;
						fringe[current_fringe] = current_tile_img;

						//go to the next tile
						current_fringe++;
						placing_fringe = false;
					}

					break;

				case COLLISION_DRAW:
					current_rect++;
					// Cleanup small rects you can't see
					SKO_Map::cleanupInvisibleRects(map);
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
