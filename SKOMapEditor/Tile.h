#ifndef __SKOMAPTILE_H_
#define __SKOMAPTILE_H_

namespace SKO_Map
{
	class Tile
	{
	public:
		Tile();
		virtual ~Tile();
		int x = 0;
		int y = 0;
		unsigned char tileId;
	};
}
#endif