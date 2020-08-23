#pragma once

#include "hexgrid.h"

struct NeighborValuesStruct
{
	int nv_[6]; // Hold 1 if the neighbor is the same as the tile, 0 otherwise
	
	NeighborValuesStruct() : nv_{0}{}
};

// Hex To Buffer Mapping
class HexToBuffer
{
	public:
	HexToBuffer(IntVector2 buffersize, IntVector2 hexmapsize);
	
	IntVector2 PointToHex(IntVector2 p);
	IntVector2 HexCenter(IntVector2 h);
	int NearestNeighborDirection(IntVector2 p);
	float BlendMaskValue(IntVector2 p, const NeighborValuesStruct &nv);
	
	protected:
	HexGrid hexgrid_;
	IntVector2 buffersize_, hexmapsize_;
	Vector2 scale_;
};
