
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

// SDL2 rendering class
OPI_Renderer *renderer = nullptr;
  
// Gui Manager
OPI_Gui::GuiManager *guiManager = nullptr;

// Map Editor Manager
SKO_MapEditor::Manager *mapEditorManager = nullptr;
//main loop
bool done = false;
  


//font.setImage("IMG/font.png");

// load map by dragging and dropping onto this executable, 
// or with command line arguments:
// SKOMapEditor.exe map0.map
std::string loadMapFilename = "";

SDL_Cursor *pointer;



std::string save = "";

char mode = 0;
bool fringe_mode = false;

void saveMap()
{
	mapEditorManager->saveMap();
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
    
   
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) 
	{
		return 1;
	}

	OPI_Timestep *timestep = new OPI_Timestep(60);
    
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
	mainMenuGui = new MainMenuGui(OPI_Gui::GuiManager::getInstance());

	// Initialize map editor manager
	mapEditorManager = new SKO_MapEditor::Manager(renderer, mainMenuGui, OPI_Gui::GuiManager::getInstance());

	// Open requested map file
	if (loadMapFilename.length() > 0)
	{
		mapEditorManager->loadMap(loadMapFilename);
	}


	//OPI_Gui::ElementThemeGridRect a = OPI_Gui::ElementThemeGridRect();

	// TODO remove this completely
	// Test out a GridRect panel
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

	while (!done)
	{
		timestep->Update();
                
		if (!done && timestep->Check())
		{ 
			mapEditorManager->processLoop();
		} 
      
		OPI_Sleep::milliseconds(1);
	} //while !done
  
  printf("DONE\n");

  delete renderer;
  SDL_Quit();
  return(0);
}