#ifndef __OPI_TEXT_H__
#define __OPI_TEXT_H__

#include <string>
#include <cstdlib>
#include "SDL/SDL_ttf"


class OPI_Text
{
      
	public:
		OPI_Text();
		void SetText(std::string content);
		size_t length();
		short int length;
		float R, G, B;
		float x, y;
		float pos_x, pos_y;
		bool used;

	private:
		std::string content;
};

#endif
