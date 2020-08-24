#include "terraingenerator.h"

#include "../hextobuffer.h"

void BuildMask(std::vector<int> &tilemap, int mapwidth, int mapheight, anl::CArray2Dd &mask, int which, float blurfactor)
{
	HexToBuffer htb(IntVector2(mask.width(), mask.height()), IntVector2(mapwidth,mapheight));
	
	std::vector<int> themap;
	for(int y=0; y<mapheight; ++y)
	{
		for(int x=0; x<mapwidth; ++x)
		{
			int tile=tilemap[y*mapwidth+x];
			
			if(tile>=which) themap.push_back(1);
			else themap.push_back(0);
		}
	}
	
	HexGrid hg(1);
	
	for(int x=0; x<mask.width(); ++x)
	{
		for(int y=0; y<mask.height(); ++y)
		{
			IntVector2 h=htb.PointToHex(IntVector2(x,y));
			int mv=themap[h.y_*mapwidth+h.x_];
			if(mv==1) mask.set(x,y, 1.0);
			else mask.set(x,y, 0.0);
		}
	}
	
	mask.blur(blurfactor, false);
	mask.scaleToRange(0,1);
}
