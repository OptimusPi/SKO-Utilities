
#include "SDL_headers.h"

#include <stdio.h> 
#include <string>  
#include <sstream> 
#include <iostream>
#include <fstream>

#include "OPI_Timestep.h"
#include "OPI_Clock.h"
#include "OPI_Sleep.h"
#include "OPI_Renderer.h"

#include "TextComponent.h"
#include "FontManager.h"
#include "GuiManager.h"
#include "Panel.h"
#include "Button.h"
#include "MessageBox.h"
#include "FontManager.h"
#include "Map.h"
#include "MapEditorManager.h"

// GUI Implementations
#include "MainMenuGui.h"
 
// TODO - get rid of Global.h
#include "Global.h"
 
MainMenuGui* mainMenuGui = nullptr;
SKO_Map::Map *map = nullptr;

// TODO enum
const char
TILE_DRAW = 1, TILE_DELETE = 2,
TOGGLE_FRINGE = 3,
COLLISION_DRAW = 4, COLLISION_DELETE = 5,
TOGGLE_TEST = 6, 
EDIT_TILE = 7, EDIT_COLLISION = 8,
SAVE = 9;

char mode = TILE_DRAW;
int num_tiles = 0;
bool fringe_mode = 0;
int save_notify;
bool SHIFT_HELD = false;
bool stickman_toggle = false;

SDL_Rect stickman;
float x_speed, y_speed;
const float GRAVITY = 0.175;
bool RIGHT = false, LEFT = false;

// SDL2 rendering class
OPI_Renderer *renderer;
  
// Gui Manager
OPI_Gui::GuiManager *guiManager = nullptr;

// Map Editor Manager
SKO_MapEditor::Manager *mapEditorManager = nullptr;

//mouse buttons
bool RCLICK = false;
bool LCLICK = false;
  
bool placing_tile = false;
bool placing_fringe = false;
//main loop
bool done = false;
  
unsigned long int coordsTicker = 0;

//font.setImage("IMG/font.png");

// load map by dragging and dropping onto this executable, 
// or with command line arguments:
// SKOMapEditor.exe map0.map
std::string loadMapFilename = "";

SDL_Cursor *pointer;


void Physics();

std::string save = "";

	for (OPI_Gui::Element* child : gui->children) {
		if (child->isVisible)
			DrawElement(0, 0, child);
	}
}

void Graphics()
{
	glClearColor(0.00f, 0.00f, 0.00f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	// Draw map, tiles, etc
	DrawGameScene();
	
	mapEditorManager->DrawGameScene();

	// Draw Map Editor Gui
	mapEditorManager->DrawGui();

	//update screen
	renderer->updateScreen();
}

#ifdef _WIN32
#include <shellapi.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	LPWSTR *szArgList;
	int argCount;

	szArgList = CommandLineToArgvW(GetCommandLine(), &argCount);
	if (szArgList[1] != NULL)
	{
		std::wstring ws(szArgList[1]);
		loadMapFilename +=  std::string(ws.begin(), ws.end());
	}
	LocalFree(szArgList);


#else

int main(int argc, char *argv[])
{

	// load map by dragging and dropping onto this executable, 
	// or with command line arguments:
	// SKOMapEditor.exe map0.map
	std::string loadMapFilename = "";
	if (argc > 1)
	{
		loadMapFilename += (std::string)argv[1];
	}

#endif

	// Open requested map file
	if (loadMapFilename.length() > 0)
	{ 
		map = new SKO_Map::Map(loadMapFilename);
		current_tile = number_of_tiles;
		current_rect = number_of_rects;
		current_fringe = number_of_fringe;
	} 
    
    OPI_Timestep *timestep = new OPI_Timestep(60);
    
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {

		return 1;
	}


	//TODO - Singleton with cached fonts
	//TODO - load from config file
	if (!OPI_Text::FontManager::init("fonts/RobotoMono-Regular.ttf"))
	{
		printf("Could not initialize SDL_ttf!\n");
	}

	OPI_Text::FontManager::addFont("RobotoMono-Regular", "fonts/RobotoMono-Regular.ttf");


	// TODO auto detect screen size, make smaller if less than 1080p to render.
	//renderer = new OPI_Renderer("SKO Map Editor v 0.9.0", 1280, 720);
	renderer = new OPI_Renderer("SKO Map Editor v 0.9.0", 1920, 1080);
	renderer->initScreen();
	OPI_Gui::GuiManager::create(renderer);
	OPI_Gui::GuiManager::initCursors("IMG/GUI/cursors/normal.png", "IMG/GUI/cursors/move.png", "IMG/GUI/cursors/resize.png", "IMG/GUI/cursors/hourglass.png", "IMG/GUI/cursors/hand.png");
	
	// Initialize main menu GUI
	mainMenuGui = new MainMenuGui(OPI_Gui::GuiManager::getInstance(), renderer);

	//OPI_Gui::ElementThemeGridRect a = OPI_Gui::ElementThemeGridRect();

	////TODO remove this completely
	//// Test out a GridRect panel
	//auto *panel_GridRect = new OPI_Gui::Panel(OPI_Gui::ElementThemeType::GridRect, "ice", 100, 100, 433, 433);
	//panel_GridRect->isVisible = true;
	//panel_GridRect->isClosable = true;
	//panel_GridRect->isResizable = true;
	//panel_GridRect->isMovable = true;
	//gui->addElement(panel_GridRect);

	//// Test out an Image panel
	//auto *panel_Image = new OPI_Gui::Panel(OPI_Gui::ElementThemeType::Image, "vapor", 51, 51);
	//panel_Image->isVisible = true;
	//panel_Image->isResizable = false;
	//panel_Image->isClosable = true;
	//panel_Image->isMovable = true;
	//panel_GridRect->addElement(panel_Image);

	//// Test out button with normal image set render
	//auto *button_Image = new OPI_Gui::Button("default", 15, 15, "Okay");
	//button_Image->isVisible = true;
	//button_Image->isEnabled = true;
	//panel_Image->addElement(button_Image);

	//// Test out MessageBox
	//auto testFont = OPI_Font::FontManager::getFont("RobotoMono-Regular");
	//auto *messageBoxTest = new OPI_Gui::MessageBox("401: Unauthorized.", testFont);
	//gui->addElement(messageBoxTest);

	background.setImage("IMG/back.png");
	stickman_img.setImage("IMG/stickman.png");

	/*button_Image->addCallback([]() {
		std::cout << "ButtonClickTest ran." << std::endl; 
	});*/

	//Render the text
	SDL_Color color;
	color.r = 255;
	color.g = 200;
	color.b = 200;

	//stickman
	stickman.x = 0;
	stickman.y = 0;
	stickman.w = 14;
	stickman.h = 64;

	// Load tileset
  for(int i = 0; i < 256; i++)//check if file exists, etc.
  {
     std::stringstream ss;
     ss << "IMG/TILE/tile" << i << ".png";
     std::ifstream checker (ss.str());
     if (checker.is_open())
     {
        checker.close();
        tile_img[i].setImage(ss.str());
        num_tiles = i;
     }
     else
        break;
  }


	while (!done)
	{
		timestep->Update();
                           
        if (!done && timestep->Check())
        {
			//if save notify is in the past, clear it
            if (save_notify < clock())
               save_notify = 0;
                          
            if (OPI_Clock::milliseconds() - coordsTicker  > 15)
            {  
                //draw coords
                std::stringstream ss;
                ss << "(" << cursor_x+camera_x << ", " << cursor_y+camera_y << ")";

				mainMenuGui->setCoords(ss.str());
                    
                //reset ticker
                coordsTicker = OPI_Clock::milliseconds();
            }        
                            
			Graphics();
			mapEditorManager->HandleInput();

            if (!done && stickman_toggle)
               Physics();
            
            //prevent repeats
            event.type = 0;
      }//while check
      
	  OPI_Sleep::microseconds(100);
  }//while !done
  
  printf("DONE\n");

  delete renderer;
  SDL_Quit();
  return(0);
}


bool blocked(float box1_x1, float box1_y1, float box1_x2, float box1_y2)
{
     for (int r = 0; r < number_of_rects; r++)
     {
          float box2_x1 = collision_rect[r].x;
          float box2_y1 = collision_rect[r].y;
          float box2_x2 = collision_rect[r].x + collision_rect[r].w;
          float box2_y2 = collision_rect[r].y + collision_rect[r].h;

          if (box1_x2 > box2_x1 && box1_x1 < box2_x2 && box1_y2 > box2_y1 && box1_y1 < box2_y2)
          {
             return true;
          }
     }
     return false;
}


void Physics()
{
	//fall
	if (y_speed < 10)
	{ 
		y_speed += GRAVITY;
	}

	//verical collision detection
	bool block_y = blocked(stickman.x, stickman.y+y_speed + 0 + 0.25, stickman.x + 14, stickman.y+y_speed + 63);

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
	bool block_x = blocked(stickman.x + x_speed , stickman.y , stickman.x + x_speed + 14, stickman.y + 63);

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