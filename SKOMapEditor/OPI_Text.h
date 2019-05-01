#ifndef __OPI_TEXT_H__
#define __OPI_TEXT_H__

// Standard libraries
#include <string>
#include <cstdlib>

// Platform dependent includes
#ifdef _WIN32
#include "SDL_ttf.h"
#elif defined __linux__
#include <SDL/SDL_ttf>
#elif defined __APPLE__
#include <SDL/SDL_ttf>
#endif


#include "OPI_Image.h"


class OPI_Text
{
      
	public:
		OPI_Text();
		static bool Init();
		void SetText(std::string content);
		size_t length();
		float R, G, B;
		float x, y;
		bool used;

	private:
		std::string content;
		OPI_Image contentRender;
		void generateImage(std::string text, std::string font);
};

#endif
