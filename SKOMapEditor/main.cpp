
#ifdef _WIN32
#include "SDL.h"
#include "SDL_Image.h"
#include "SDL_opengl.h" 
#else 
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_opengl.h> 
#endif

#include <stdio.h>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include "OPI_Timestep.h"
#include "OPI_Text.h"
#include "OPI_Fonts.h"
#include "OPI_Clock.h"
#include "OPI_Sleep.h"
#include "OPI_Gui.h"

//modes
const char 

TILE_DRAW = 1, TILE_DELETE = 2, 
COLLISION_DRAW = 3, COLLISION_DELETE = 4,
STICKMAN_DRAW = 5, STICKMAN_DELETE = 6,
FRINGE_TOGGLE = 7,
SAVE = 8;

char mode = TILE_DRAW;
int num_tiles = 0;
bool fringe_mode = 0;
int save_notify;
bool SHIFT_HELD = false;

SDL_Surface *screen;
SDL_Rect collision_rect[32768];
int collision_ox;
int collision_oy;
int tile_x[32768], tile_y[32768];
unsigned char tile[32768];
int fringe_x[32768], fringe_y[32768];
unsigned char fringe[32768];
int number_of_tiles = 0;
int number_of_rects = 0;
int number_of_fringe = 0;
bool stickman_toggle = false;

SDL_Rect stickman;
float x_speed, y_speed;
const float GRAVITY = 0.175;
bool RIGHT = false, LEFT = false;

// Global variables
SDL_Event event;
float camera_x = 0, camera_y = 0;
int cursor_x = 0, cursor_y = 0;
int current_tile = 0;
int current_rect = 0;
int current_fringe = 0;
int current_tile_img = 0;


  
  //mouse buttons
  bool RCLICK = false;
  bool LCLICK = false;
  
  bool placing_tile = false;
  bool placing_fringe = false;
  //main loop
  bool done = false;
  
  unsigned long int coordsTicker = 0;



void DrawImage( int x, int y, OPI_Image img) 
{      
     glColor3f(1.0f, 1.0f, 1.0f);     
     glBindTexture( GL_TEXTURE_2D,  img.texture);
      
      glBegin( GL_QUADS );
    	//Top-left vertex (corner)
    	glTexCoord2f( 0, 0 );
    	glVertex3f( x, y, 0 );
    	
    	//Bottom-left vertex (corner)
    	glTexCoord2f( 0, 1 );
    	glVertex3f( x, y+img.height, 0 );
    	
    	//Bottom-right vertex (corner)
    	glTexCoord2f( 1, 1 );
    	glVertex3f( x+img.width, y+img.height, 0 );
    	
    	//Top-right vertex (corner)
    	glTexCoord2f( 1, 0 );
    	glVertex3f( x+img.width, y, 0 );
    glEnd();
   
} 

void drawPanel(OPI_Panel *panel)
{
	if (!panel->texture)
		panel->render(screen);

	DrawImage(panel->x, panel->y, panel->texture);
}

OPI_Text* coords;



void DrawRect(SDL_Rect rect)
{
     glColor3f(0.0f, 0.0f, 1.0f);
     glBegin(GL_LINE_LOOP);
     glVertex2f(rect.x + 0.5, rect.y + 0.5);
     glVertex2f(rect.x+rect.w + 0.5, rect.y + 0.5);
     glVertex2f(rect.x+rect.w + 0.5, rect.y+rect.h + 0.5);
     glVertex2f(rect.x + 0.5, rect.y+rect.h + 0.5);
     glEnd();
}

void cleanupInvisibleRects()
{
	//Delete any collision rectangles that are too small.
	for (int i = 0; i < current_rect; i++) {
		if (collision_rect[i].h < 4 || collision_rect[i].w < 4) {
			for (; i < current_rect; i++) {
				collision_rect[i] = collision_rect[i + 1];
			}
			current_rect--;
			number_of_rects--;
		}
	}
}

void loadmap (std::string FileName)
{
     std::ifstream MapFile(FileName.c_str(), std::ios::in|std::ios::binary|std::ios::ate);
      
     if (MapFile.is_open())
     {
        //loading variables
        std::ifstream::pos_type size;
        char * memblock;  

        //allocate memory
        size = MapFile.tellg();
        memblock = (char *)malloc(size);

        //load the file into memory
        MapFile.seekg (0, std::ios::beg);
        MapFile.read (memblock, size);
        //close file
        MapFile.close();

        //hold the result...
        unsigned int num;
        
        //build an int from 4 bytes
        ((char*)&num)[0] = memblock[0];
        ((char*)&num)[1] = memblock[1];
        ((char*)&num)[2] = memblock[2];
        ((char*)&num)[3] = memblock[3];

        
        //store the number into variables
        number_of_tiles = num;              

        //build an int from 4 bytes
        ((char*)&num)[0] = memblock[4];
        ((char*)&num)[1] = memblock[5];
        ((char*)&num)[2] = memblock[6];
        ((char*)&num)[3] = memblock[7];

        //store the number into variables
        number_of_fringe = num;
        
        //build an int from 4 bytes
        ((char*)&num)[0] = memblock[8];
        ((char*)&num)[1] = memblock[9];
        ((char*)&num)[2] = memblock[10];
        ((char*)&num)[3] = memblock[11];

        //store the number into variables
        number_of_rects = num;
        

        //
        //tiles
        //
        int last_i = 11;
        
        for (int i = 0; i < number_of_tiles; i++)
        {
            //9 bytes per tile silly ;)
            
            //build an int from 4 bytes
            ((char*)&num)[0] = memblock[last_i+1+i*9];
            ((char*)&num)[1] = memblock[last_i+2+i*9];
            ((char*)&num)[2] = memblock[last_i+3+i*9];
            ((char*)&num)[3] = memblock[last_i+4+i*9];

            //store the number into variables
            tile_x[i] = num;


            //build an int from 4 bytes
            ((char*)&num)[0] = memblock[last_i+5+i*9];
            ((char*)&num)[1] = memblock[last_i+6+i*9];
            ((char*)&num)[2] = memblock[last_i+7+i*9];
            ((char*)&num)[3] = memblock[last_i+8+i*9];

            //store the number into variables
            tile_y[i] = num;

            //store the number into variables
            tile[i] = memblock[last_i+9+i*9];
            
            
        }

        last_i += number_of_tiles*9;
        //
        //fringe tiles
        //
        for (int i = 0; i < number_of_fringe; i++)
        {
            //9 bytes per tile silly ;)
            
            //build an int from 4 bytes
            ((char*)&num)[0] = memblock[last_i+1+i*9];
            ((char*)&num)[1] = memblock[last_i+2+i*9];
            ((char*)&num)[2] = memblock[last_i+3+i*9];
            ((char*)&num)[3] = memblock[last_i+4+i*9];

            //store the number into variables
            fringe_x[i] = num;


            //build an int from 4 bytes
            ((char*)&num)[0] = memblock[last_i+5+i*9];
            ((char*)&num)[1] = memblock[last_i+6+i*9];
            ((char*)&num)[2] = memblock[last_i+7+i*9];
            ((char*)&num)[3] = memblock[last_i+8+i*9];

            //store the number into variables
            fringe_y[i] = num;

            //store the number into variables
            fringe[i] = memblock[last_i+9+i*9];
            
            
        }
        last_i += number_of_fringe * 9;
        //
        //rects
        //
        for (int i = 0; i < number_of_rects; i++)
        {
            //read the map file
            //build an int from 4 bytes
            ((char*)&num)[0] = memblock[last_i+1+i*16];
            ((char*)&num)[1] = memblock[last_i+2+i*16];
            ((char*)&num)[2] = memblock[last_i+3+i*16];
            ((char*)&num)[3] = memblock[last_i+4+i*16];

            //store the number into variables
            collision_rect[i].x = num;


            //build an int from 4 bytes
            ((char*)&num)[0] = memblock[last_i+5+i*16];
            ((char*)&num)[1] = memblock[last_i+6+i*16];
            ((char*)&num)[2] = memblock[last_i+7+i*16];
            ((char*)&num)[3] = memblock[last_i+8+i*16];

            //store the number into variables
            collision_rect[i].y = num;


            //build an int from 4 bytes
            ((char*)&num)[0] = memblock[last_i+9+i*16];
            ((char*)&num)[1] = memblock[last_i+10+i*16];
            ((char*)&num)[2] = memblock[last_i+11+i*16]; 
            ((char*)&num)[3] = memblock[last_i+12+i*16];

            //store the number into variables
            collision_rect[i].w = num;
            
            //build an int from 4 bytes
            ((char*)&num)[0] = memblock[last_i+13+i*16];
            ((char*)&num)[1] = memblock[last_i+14+i*16];
            ((char*)&num)[2] = memblock[last_i+15+i*16];
            ((char*)&num)[3] = memblock[last_i+16+i*16];


            //store the number into variables
            collision_rect[i].h = num;
         }

         free(memblock);
     }

	 cleanupInvisibleRects();
}

void screenOptions()
{
    SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 1);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    

    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    glViewport( 0, 0, 1024, 600 );

    glClear( GL_COLOR_BUFFER_BIT );

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    glOrtho(0.0f, 1024, 600, 0.0f, -1.0f, 1.0f);

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    

    glDisable(GL_DEPTH_TEST);
    

    SDL_WM_SetCaption ("Stick Knights Online - Map Editor v8", NULL);
}

void Physics();
void drawText(OPI_Text *text);

std::string save = "";




#ifdef _WIN32
#include <shellapi.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{

	// load map by dragging and dropping onto this executable, 
	// or with command line arguments:
	// SKOMapEditor.exe map0.map
	std::string loadMapFilename = "";

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
		loadmap(loadMapFilename);
		current_tile = number_of_tiles;
		current_rect = number_of_rects;
		current_fringe= number_of_fringe;
	}
    
    OPI_Timestep *timestep = new OPI_Timestep(20000);
    
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
        return 1;
         
    screen = SDL_SetVideoMode( 1024, 600, 32, SDL_OPENGL);
     
    screenOptions();      

	//Render the text
	SDL_Color color;
	color.r = 255;
	color.g = 200;
	color.b = 200;

	//TODO - load from config file
	OPI_Fonts *fonts = new OPI_Fonts(); 
	if (!fonts->init())
	{
		printf("Could not initialize SDL_ttf!\n");
	}

	fonts->addFont("RobotoMono-Regular", "fonts/RobotoMono-Regular.ttf");
	coords = new OPI_Text("1000, 400", fonts->getFont("RobotoMono-Regular"));

	//stickman
	stickman.x = 0;
	stickman.y = 0;
	stickman.w = 14;
	stickman.h = 64;
  
	SDL_ShowCursor(0);

	//coords text
	coords->setText("0,0", NULL);
	coords->visible = true;
	coords->x = 2;
	coords->y = 2;
	coords->R = 1.0f;
	coords->G = 1.0f;
	coords->B = 1.0f;
     
     
     
  //images
  OPI_Image tile_img[256], cursor;
  OPI_Image background;
  OPI_Image gui, selector;
  OPI_Image stickman_img;
  cursor.setImage("IMG/cursor.png");
  background.setImage("IMG/back.png");
  gui.setImage("IMG/gui.png");
  selector.setImage("IMG/selector.png");
  stickman_img.setImage("IMG/stickman.png");
  //font.setImage("IMG/font.png");
  
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

  OPI_Panel *testPanel = new OPI_Panel("ice", 100, 100, 100, 80);

  while (!done)
  {
        timestep->Update();
                           
        while (!done && timestep->Check())
        {
			//if save notify is in the past, clear it
            if (save_notify < clock())
               save_notify = 0;
                          
                if (OPI_Clock::milliseconds() - coordsTicker  > 1)
                {  
                    //draw coords
                    std::stringstream ss;
                    ss << "(" << cursor_x+camera_x << ", " << cursor_y+camera_y << ")";
                    coords->setText((char *)ss.str().c_str());
                    
                    //reset ticker
                    coordsTicker = OPI_Clock::milliseconds();
                }        
                          
                          
                               
               //image buffer and background                    
               DrawImage(0, 0, background);
                
               //draw tiles, only on screen
               for (int i = 0; i < number_of_tiles; i++)
               {
    
                   int draw_x = tile_x[i] - (int)camera_x;
                   int draw_y = tile_y[i] - (int)camera_y;
                   
                   if (draw_x >= 0-tile_img[tile[i]].width &&
                      draw_x < 1024 && draw_y < 600 &&
                      draw_y >= 0-tile_img[tile[i]].height)
                   DrawImage(draw_x, draw_y, tile_img[tile[i]]);
               }
               
                  
               //stickman!
               if (stickman_toggle)
                  DrawImage(stickman.x - 25 - camera_x, stickman.y - camera_y, stickman_img);
               
               
               
                 
               //draw tiles, only on screen
               for (int i = 0; i < number_of_fringe; i++)
               {
    
                   int draw_x = fringe_x[i] - (int)camera_x;
                   int draw_y = fringe_y[i] - (int)camera_y;
                   
                   if (draw_x >= 0-tile_img[fringe[i]].width &&
                      draw_x < 1024 && draw_y < 600 &&
                      draw_y >= 0-tile_img[fringe[i]].height)
                   DrawImage(draw_x, draw_y, tile_img[fringe[i]]);
               }
               
               
               //draw current tile
               if (mode == TILE_DRAW)
                  DrawImage(cursor_x/32*32, cursor_y/32*32, tile_img[current_tile_img]);
                          
               
            
                        
               //draw collision rects, only on screen  
               for (int i = 0; i < number_of_rects; i++)
               {   
    
                   SDL_Rect newRect;
                   
                   newRect.x = collision_rect[i].x - (int)camera_x;
                   newRect.y = collision_rect[i].y - (int)camera_y;
                   newRect.h = collision_rect[i].h;
				   newRect.w = collision_rect[i].w;
                   
                   if (newRect.x >= 0-collision_rect[i].w &&
                       newRect.x < 1024 && newRect.y < 600 &&
                       newRect.y >= 0-collision_rect[i].h)
                       
                   DrawRect(newRect);
               }
               
               //draw bottom gui bar
               DrawImage(144, 536, gui);
               
               //draw gui selector
               DrawImage(144 + (mode-1)*64, 536, selector);
               
               if (fringe_mode)
                  DrawImage(144 + (FRINGE_TOGGLE-1)*64, 536, selector);
               
               if (save_notify)
                  DrawImage(144 + (SAVE-1)*64, 536, selector);
                  
               if (!stickman_toggle)
                  DrawImage(144 + (STICKMAN_DELETE-1)*64, 536, selector);
               

			   //draw GUI
			   drawPanel(testPanel);
               
               //draw coords
               drawText(coords);
               
               //draw cursor
               DrawImage(cursor_x, cursor_y, cursor);
               
               
               //update screen
               SDL_GL_SwapBuffers();
 
        
            while (!done && SDL_PollEvent(&event))
            {        
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
                                   mode = COLLISION_DRAW;
                           break;
                           case '4':
                                   mode = COLLISION_DELETE;
                           break;
                           case '5':
                                   mode = STICKMAN_DRAW;
                           break;
                           case '6':
                                   stickman_toggle = !stickman_toggle;
                           break;
                           case '7':
                                   fringe_mode = !fringe_mode;
                           break;
                           case '8':
                                 {  
									// Cleanup small rects you can't see
									cleanupInvisibleRects();

                                    //show selector for a second
                                    save_notify = clock() + 120;
                                    
                                    //default
                                    std::string file = "Output.map";
                                    
                                    //unless you have opened one
                                    if (loadMapFilename.length() > 1)
                                       file = loadMapFilename; 
                                    
                                    //dump all the memory into a file
                                    std::ofstream MapFile(file.c_str(), std::ios::out|std::ios::binary);
                                    
                                    if (MapFile.is_open())
                                    {     
                                          
                                       //first say how many tiles
                                       //break up the int as 4 bytes
                                       unsigned char *p;
                                       p=(unsigned char*)&number_of_tiles;
                                       unsigned char b1=p[0];
                                       unsigned char b2=p[1];
                                       unsigned char b3=p[2];
                                       unsigned char b4=p[3];
                                       
                                       //spit out each of the bytes
                                       MapFile << b1 << b2 << b3 << b4;
                                       
                                       //put how many fringe
                                       p=(unsigned char*)&number_of_fringe;
                                       b1=p[0]; b2=p[1]; b3=p[2]; b4=p[3];
                                       
                                       //spit out each of the bytes
                                       MapFile << b1 << b2 << b3 << b4;
                                       
                                       //put how many rects
                                       p=(unsigned char*)&number_of_rects;
                                       b1=p[0]; b2=p[1]; b3=p[2]; b4=p[3];
                                       
                                       //spit out each of the bytes
                                       MapFile << b1 << b2 << b3 << b4;
                                       
                                       
                                       //spit out all the tiles
                                       for (int i = 0; i < number_of_tiles; i++)   
                                       {
                                           //x coords
                                           p=(unsigned char*)&tile_x[i];
                                           b1=p[0]; b2=p[1]; b3=p[2]; b4=p[3];
                                       
                                           //spit out each of the bytes
                                           MapFile << b1 << b2 << b3 << b4;
                                           
                                           //y coords
                                           p=(unsigned char*)&tile_y[i];
                                           b1=p[0]; b2=p[1]; b3=p[2]; b4=p[3];
                                       
                                           //spit out each of the bytes
                                           MapFile << b1 << b2 << b3 << b4;
                                           
                                           //tile number
                                           MapFile << tile[i];   
                                             
                                       }
                                       
                                       //spit out all the tiles
                                       for (int i = 0; i < number_of_fringe; i++)   
                                       {
                                           //x coords
                                           p=(unsigned char*)&fringe_x[i];
                                           b1=p[0]; b2=p[1]; b3=p[2]; b4=p[3];
                                       
                                           //spit out each of the bytes
                                           MapFile << b1 << b2 << b3 << b4;
                                           
                                           //y coords
                                           p=(unsigned char*)&fringe_y[i];
                                           b1=p[0]; b2=p[1]; b3=p[2]; b4=p[3];
                                       
                                           //spit out each of the bytes
                                           MapFile << b1 << b2 << b3 << b4;
                                           
                                           //fringe number
                                           MapFile << fringe[i];   
                                             
                                       }
                                       
                                       //spit out all the rects
                                       for (int i = 0; i < number_of_rects; i++)   
                                       {
                                           //x coords
                                           p=(unsigned char*)&collision_rect[i].x;
                                           b1=p[0]; b2=p[1]; b3=p[2]; b4=p[3];
                                       
                                           //spit out each of the bytes
                                           MapFile << b1 << b2 << b3 << b4;
                                           
                                           //y coords
                                           p=(unsigned char*)&collision_rect[i].y;
                                           b1=p[0]; b2=p[1]; b3=p[2]; b4=p[3];
                                       
                                           //spit out each of the bytes
                                           MapFile << b1 << b2 << b3 << b4;
                                           
                                           //width
                                           p=(unsigned char*)&collision_rect[i].w;
                                           b1=p[0]; b2=p[1]; b3=p[2]; b4=p[3];
                                       
                                           //spit out each of the bytes
                                           MapFile << b1 << b2 << b3 << b4;
                                           
                                           //height
                                           p=(unsigned char*)&collision_rect[i].h;
                                           b1=p[0]; b2=p[1]; b3=p[2]; b4=p[3];
                                       
                                           //spit out each of the bytes
                                           MapFile << b1 << b2 << b3 << b4;     
                                       }
                                       
                                       MapFile.close(); 
                                    }
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
                                     camera_y+=32;
                           break;
                           case  SDLK_RIGHT: 
                                 if (SHIFT_HELD)
                                    camera_x += 1024;
                                 else
                                     camera_x+=32;
                           break;
                           
                           
                           
                           
                           case SDLK_HOME:
                                camera_x = 0;
                                camera_y = 0;
                           break; 
                           case SDLK_PAGEUP:
                                if (current_tile_img > 0)
                                   current_tile_img --;
                           break;
                           
                           case SDLK_PAGEDOWN:
                                if (current_tile_img < num_tiles)
                                   current_tile_img ++;
                           break;
                    
                           case 'w':    
                                      if (current_tile > 0&& mode == TILE_DRAW && !fringe_mode) 
                                         tile_y[current_tile-1]--;  
                                      if (current_fringe > 0&& mode == TILE_DRAW && fringe_mode) 
                                         fringe_y[current_fringe-1]--;   
                                         
                                      if (mode == STICKMAN_DRAW)
                                          y_speed = -6; 
                                      if (mode == COLLISION_DRAW && current_rect > 0) 
                                      {
                                               if (collision_rect[current_rect - 1].y > 0)
                                               {
                                                   collision_rect[current_rect - 1].y --;
                                                   collision_rect[current_rect - 1].w ++;
                                               }
                                      }
                           break;
                           case 'a':  
                                      
                                      if (current_tile > 0 && mode == TILE_DRAW && !fringe_mode)
                                      { 
                                         tile_x[current_tile-1]--;   
                                      }
                                      if (current_fringe > 0&& mode == TILE_DRAW && fringe_mode) 
                                      {
                                         fringe_x[current_fringe-1]--;   
                                      }
                                         
                                       LEFT = true;
                                      
                                      if (mode == COLLISION_DRAW && current_rect > 0) 
                                      {
                                               if (collision_rect[current_rect - 1].x > 0)
                                               {
                                                   collision_rect[current_rect - 1].x --;
                                                   collision_rect[current_rect - 1].w ++;
                                               }
                                      }
                           break;
                           case 's':  
                                      if (current_tile > 0&& mode == TILE_DRAW && !fringe_mode) 
                                          tile_y[current_tile-1]++;   
                                      if (current_fringe > 0&& mode == TILE_DRAW && fringe_mode) 
                                         fringe_y[current_fringe-1]++;   
                                           
                                      if (mode == COLLISION_DRAW && current_rect > 0) 
                                      {
                                         collision_rect[current_rect - 1].y ++;   
                                         collision_rect[current_rect - 1].h --; 
                                      }
                                          
                           break;
                           case 'd': 
                                     
                                      if (current_tile > 0 && mode == TILE_DRAW && !fringe_mode) 
                                         tile_x[current_tile-1]++;   
                                      if (current_fringe > 0 && mode == TILE_DRAW && fringe_mode) 
                                         fringe_x[current_fringe-1]++;   
                                      
                                      RIGHT = true; 
                                       
                                      if (mode == COLLISION_DRAW && current_rect > 0) 
                                      {
                                         collision_rect[current_rect - 1].x ++;
                                         collision_rect[current_rect - 1].w --;
                                      }
                           break;
                          
                           //smaller rects
                            case 'i': 
                                      if (mode == COLLISION_DRAW && current_rect > 0) 
                                      {
                                               if (collision_rect[current_rect - 1].h > 0)
                                               {
                                                   collision_rect[current_rect - 1].h --;
                                               }
                                      }
                           break;
                           case 'j':  
                                      if (mode == COLLISION_DRAW && current_rect > 0) 
                                      {
                                               if (collision_rect[current_rect - 1].w > 0)
                                               {
                                                   collision_rect[current_rect - 1].w --;
                                               }
                                      }
                           break;
                           case 'k':   
                                      if (mode == COLLISION_DRAW && current_rect > 0) 
                                      {
                                         collision_rect[current_rect - 1].h ++;  
                                      } 
                           break;
                           case 'l': 
                                      if (mode == COLLISION_DRAW && current_rect > 0) 
                                      {
                                         collision_rect[current_rect - 1].w ++;
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
                   
                if( event.type == SDL_MOUSEMOTION)
                {
                    //Get the mouse offsets
                    cursor_x = event.motion.x;
                    cursor_y = event.motion.y;
                    
                    //printf("x: %i, y: %i\n", cursor_x, cursor_y);
                    
                    //draw the almost done rect
                    if (mode == COLLISION_DRAW && LCLICK)
                    {
                       //variables for easy determining
                       int cx = cursor_x+(int)camera_x;
                       int cy = cursor_y+(int)camera_y;
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
                       collision_rect[current_rect].w = x2-x1;
                       collision_rect[current_rect].y = y1;
                       collision_rect[current_rect].h = y2-y1;
                    }
                    if (mode == TILE_DRAW && LCLICK)
                    {
                       if (!fringe_mode)
                       {
                           tile_x[current_tile] = (int)(cursor_x+camera_x)/32*32;
                           tile_y[current_tile] = (int)(cursor_y+camera_y)/32*32;
                       }
                       else
                       {
                           fringe_x[current_fringe] = (int)(cursor_x+camera_x)/32*32;
                           fringe_y[current_fringe] = (int)(cursor_y+camera_y)/32*32;
                       }
                    }
                    if (mode == STICKMAN_DRAW && LCLICK)
                    {
                       stickman.x = cursor_x + camera_x;
                       stickman.y = cursor_y + camera_y;
                    }
                }
         
                if ( event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && !LCLICK)
                {      
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
                                  
                               tile_x[current_tile] = (int)(cursor_x+(int)camera_x)/32*32;
                               tile_y[current_tile] = (int)(cursor_y+(int)camera_y)/32*32;
                               tile[current_tile] = current_tile_img;
                           }
                           else
                           {
                               if (!placing_fringe)   
                               {              
                                  number_of_fringe++;
                                  placing_fringe = true;
                               }
                                  
                               fringe_x[current_fringe] = (int)(cursor_x+(int)camera_x)/32*32;
                               fringe_y[current_fringe] = (int)(cursor_y+(int)camera_y)/32*32;
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
                                   if (x > tile_x[i] && x < tile_x[i]+tile_img[tile[i]].width &&
                                       y > tile_y[i] && y < tile_y[i]+tile_img[tile[i]].height)
                                   {
                                      found = true;
                                      break;
                                   }
                               }
                               
                               //move all tiles back in the array
                               for (; found && i < current_tile; i++)
                               {
                                   tile[i]   = tile[i+1];
                                   tile_x[i] = tile_x[i+1];
                                   tile_y[i] = tile_y[i+1];
                               }
                               
                               //we deleted a tile. current position is one less now.
                               if (found){
                                  current_tile --;
                                  number_of_tiles --;
                               }
                           }
                           else
                           {
                               bool found = false;
                               for (i = 0; i < current_fringe; i++)
                               {
                                   if (x > fringe_x[i] && x < fringe_x[i]+tile_img[fringe[i]].width &&
                                       y > fringe_y[i] && y < fringe_y[i]+tile_img[fringe[i]].height)
                                   {
                                      found = true;
                                      break;
                                   }
                               }
                               
                               //move all tiles back in the array
                               for (; found && i < current_fringe; i++)
                               {
                                   fringe[i]   = fringe[i+1];
                                   fringe_x[i] = fringe_x[i+1];
                                   fringe_y[i] = fringe_y[i+1];
                               }
                               
                               //we deleted a tile. current position is one less now.
                               if (found){
                                  current_fringe --;
                                  number_of_fringe --;
                               }
                           }
                      }
                      break; 
                      
                      case COLLISION_DRAW:
                            //go to next rect
                            number_of_rects++;
                           
                            //Get the mouse offsets
                            collision_ox = event.button.x+(int)camera_x;
                            collision_oy = event.button.y+(int)camera_y;
                            collision_rect[current_rect].x = collision_ox;
                            collision_rect[current_rect].y = collision_oy;
                            collision_rect[current_rect].w = 0;
                            collision_rect[current_rect].h = 0;
                      break;
                       
                      case COLLISION_DELETE:{
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
                               collision_rect[i] = collision_rect[i+1];
                               
                           }
                           
                           
                           //we deleted a rect. current position is one less now.
                           if (found){
                              current_rect --;
                              number_of_rects --;
                           }
                      }
                      break;  
                      
                      
                      case STICKMAN_DRAW:
                       stickman_toggle = true;    
                       stickman.x = cursor_x + camera_x;
                       stickman.y = cursor_y + camera_y;
                    
                      break;
                      
                      
                     }
                     
                     LCLICK = true;
                }

                if ( event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_RIGHT && !RCLICK)
                {
                    RCLICK = true;
                    //TODO - use RCLICK for collision rect editing
                }
                if( event.type == SDL_MOUSEBUTTONUP ) 
                {         
                    if ( event.button.button == SDL_BUTTON_LEFT && LCLICK)
                    {
                           switch (mode)
                           {
                               case TILE_DRAW:
                                    if (!fringe_mode)
                                    {
                                        tile_x[current_tile] = (int)(event.button.x+(int)camera_x)/32*32;
                                        tile_y[current_tile] = (int)(event.button.y+(int)camera_y)/32*32;
                                        tile[current_tile] = current_tile_img;
                                   
                                        //go to the next tile
                                        current_tile++; 
                                        placing_tile = false; 
                                    }
                                    else 
                                    {
                                        fringe_x[current_fringe] = (int)(event.button.x+(int)camera_x)/32*32;
                                        fringe_y[current_fringe] = (int)(event.button.y+(int)camera_y)/32*32;
                                        fringe[current_fringe] = current_tile_img;
                                   
                                        //go to the next tile
                                        current_fringe++; 
                                        placing_fringe = false; 
                                    } 
                                   
                               break;
                               
                               case COLLISION_DRAW:
                                     current_rect++;
									 // Cleanup small rects you can't see
									 cleanupInvisibleRects();
                               break;
                           }
                                   
                          LCLICK = false;
                   }

                   if ( event.button.button == SDL_BUTTON_RIGHT && RCLICK)
                   {
                       RCLICK = false;
                   }
               }
            } //if poll event
    
            if (!done && stickman_toggle)
               Physics();
            
            //prevent repeats
            event.type = 0;
      }//while check
      
      SDL_Delay(1);
  }//while !done
  
  //printf("number_of_tiles: %i", number_of_tiles);
  
  printf("DONE\n");
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
            // printf("collision with rect[%i]\n\tme:\t{%i,%i,%i,%i}\n\tit:\t{%i,%i,%i,%i}\n", r, box1_x1, box1_y1, box1_x2, box1_y2, box2_x1, box2_y1, box2_x2, box2_y2);
            // printf("%i > %i && %i < %i && %i > %i && %i < %i\n\n", box1_x2, box2_x1, box1_x1, box2_x2, box1_y2, box2_y1, box1_y1, box2_y2);
             return true;

          }
     }
     return false;
}

void Physics()
{
              //fall
             if (y_speed < 10){ 
                 y_speed += GRAVITY;
             }

             //verical collision detection
             bool block_y = blocked(stickman.x, stickman.y+y_speed + 0 + 0.25, stickman.x + 14, stickman.y+y_speed + 63);


             //vertical movement
             if (!block_y)
             {//not blocked, fall

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
             {//not blocked, walk
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


void drawText(OPI_Text *text)
{
	//glBindTexture(GL_TEXTURE_2D, font.texture);
         
	//tint
	glColor3f(text->R, text->G, text->B);
         
	float screen_x = text->x;
	float screen_y = text->y;
    DrawImage(10, 10, text->contentRender);

	//reset tint
	glColor3f(1.0f, 1.0f, 1.0f);
}
