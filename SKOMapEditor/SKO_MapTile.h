#ifndef __SKOMAPTILE_H_
#define __SKOMAPTILE_H_

namespace SKO_Map
{
	class Tile
	{
	public:
		Tile();
		Tile(int x, int y, unsigned char tileId);
		virtual ~Tile();
		int x = 0;
		int y = 0;
		unsigned char tileId;
	};
}
#endif