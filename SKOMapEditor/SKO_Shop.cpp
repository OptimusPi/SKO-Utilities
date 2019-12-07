#include "SKO_Shop.h"

SKO_Shop::SKO_Shop()
{
	clearItems();
}

SKO_Shop::SKO_Shop(std::string title)
{
	this->title = title;
	clearItems();
}


void SKO_Shop::clearItems()
{
	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 6; x++)
		{
			//item 0
			item[x][y][0] = 0;

			//buy for 0 (dont have!!)
			item[x][y][1] = 0;
		}
	}
}