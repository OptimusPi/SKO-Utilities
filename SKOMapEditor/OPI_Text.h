#ifndef __OPI_TEXT_H__
#define __OPI_TEXT_H__

#include <string>
#include <cstdlib>



class OPI_Text
{
      
	public:
		OPI_Text();
		void SetText(std::string content);
		unsigned int length();
		short int length;
		float R, G, B;
		float x, y;
		float pos_x, pos_y;
		bool used;

	private:
		std::string content;
};

#endif
