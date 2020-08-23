#include "hexgrid.h"

HexGrid::HexGrid(float hexside)
{
	SetHexSide(hexside);
}

void HexGrid::SetHexSide(float hexside)
{
	hexside_=hexside;
	hexheight_=2.0f*hexside;
	rowheight_=1.5f*hexside;
	halfwidth_=sqrtf(hexside*hexside - (hexside*hexside)/4.0f);
	hexwidth_=halfwidth_*2.0f;
}

Vector2 HexGrid::CalcTileOrigin(int tx, int ty)
{
	Vector2 origin;
    float ymod=fmod(ty, 2.0f);
    if(ymod==1.0f) ymod=halfwidth_;
    else ymod=0.0f;
    origin=Vector2((float)tx*hexwidth_+ymod,(float)ty*rowheight_);
    return origin;
}

Vector2 HexGrid::CalcTileCenter(int tx, int ty)
{
	Vector2 c=CalcTileOrigin(tx,ty);
    Vector2 center(c.x_+halfwidth_,c.y_+hexheight_*0.5f);
	return center;
}

Vector2 HexGrid::CalcTileBottom(int tx, int ty)
{
	Vector2 c=CalcTileOrigin(tx,ty);
    Vector2 center;
    center=Vector2(c.x_+halfwidth_, c.y_+hexheight_);
    return center;
}

IntVector2 HexGrid::CalcPointTile(float px, float py)
{
	IntVector2 tile;
    float rise=hexheight_-rowheight_;
    float slope=rise/halfwidth_;
    int X=(int)(px/hexwidth_);
    int Y=(int)(py/rowheight_);

    float offsetX=px-(float)X*hexwidth_;
    float offsetY=py-(float)Y*rowheight_;

    if(fmod(Y, 2)==0)
    {
        if(offsetY < (-slope * offsetX+rise))
        {
            --X;
            --Y;
        }
        else if(offsetY < (slope*offsetX-rise))
        {
            --Y;
        }
    }
    else
    {
        if(offsetX >= halfwidth_)
        {
            if(offsetY <(-slope*offsetX+rise*2))
            {
                --Y;
            }
        }
        else
        {
            if(offsetY<(slope*offsetX))
            {
                --Y;
            }
            else
            {
                --X;
            }
        }
    }
    tile.x_=X;
    tile.y_=Y;
    return tile;
}

Vector2 HexGrid::CalcMapArea(int sx, int sy)
{
	float w=((float)sx*2.0f + (sy>1 ? 1.0f : 0.0f))*halfwidth_;
	float h=(float)sy*rowheight_ + (rowheight_ / 3.0f);
	return Vector2(w,h);
}

int HexGrid::RotateDirection(int dir, int amount)
{
	if(dir<0 || dir>5) return dir;

    dir+=amount;
    int n_dir=dir % NumberOfDirections;
    if(n_dir<0) n_dir=NumberOfDirections+n_dir;
    return n_dir;
}

int HexGrid::OppositeDirection(int dir)
{
	return RotateDirection(dir, 3);
}

int HexGrid::CalcDistance(int sx, int sy, int dx, int dy)
{
	// Convert to Skewed Space
    sx=sx-sy/2;
    dx=dx-dy/2;

    int deltax=dx-sx;
    int deltay=dy-sy;

    return (abs(deltax)+abs(deltay)+abs(deltax+deltay))/2;
}

IntVector2 HexGrid::CalcNeighbor(int tx, int ty, int direction)
{
	if(direction<0) direction=0;
	if(direction>5) direction=5;
	static IntVector2 dirs1[6]=
	{
		{0,1},
		{-1,0},
		{0,-1},
		{1,-1},
		{1,0},
		{1,1}
	};
	static IntVector2 dirs0[6]=
	{
		{-1,1},
		{-1,0},
		{-1,-1},
		{0,-1},
		{1,0},
		{0,1}
	};

	if(ty%2==0)
	{
		return IntVector2(tx,ty) + dirs0[direction];
	}
	else return IntVector2(tx,ty) + dirs1[direction];
}
