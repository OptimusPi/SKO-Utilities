#ifndef __OPI_TEXT_H__
#define __OPI_TEXT_H__

#include <string>
#include <cstdlib>

#ifdef _WIN32
#include "SDL_ttf.h"
#else
#include <SDL/SDL_ttf>
#endif



class OPI_Text
{
      
	public:
		OPI_Text();
		void SetText(std::string content);
		size_t length();
		float R, G, B;
		float x, y;
		bool used;

	private:
		std::string content;
};

#endif
