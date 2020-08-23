#include "hextobuffer.h"
#include <cmath>

Vector2 NearestPoint(Vector2 v, Vector2 p)
{
	// Find the nearest point on v to p
	float len=v.Length();
	float u=(p.x_*v.x_ + p.y_*v.y_)/len;
	return v*u;
}

Vector3 BarycentricCoords(Vector2 v1, Vector2 v2, Vector2 v3, Vector2 p)
{
	float denom=(v2.y_ - v3.y_)*(v1.x_ -v3.x_) + (v3.x_ - v2.x_)*(v1.y_-v3.y_);
	
	float w1=((v2.y_ - v3.y_)*(p.x_ - v3.x_) + (v3.x_ - v2.x_)*(p.y_ - v3.y_))/denom;
	float w2=((v3.y_ - v1.y_)*(p.x_ - v3.x_) + (v1.x_ - v3.x_)*(p.y_ - v3.y_))/denom;
	float w3=1.0f - w1 - w2;
	return Vector3(w1,w2,w3);
}

HexToBuffer::HexToBuffer(IntVector2 buffersize, IntVector2 hexmapsize) : hexgrid_(1), buffersize_(buffersize), hexmapsize_(hexmapsize)
{
	Vector2 area=hexgrid_.CalcMapArea(hexmapsize.x_, hexmapsize.y_);
	
	scale_.y_ = (float)buffersize.y_ / area.y_;
	scale_.x_ = scale_.y_;
}

IntVector2 HexToBuffer::PointToHex(IntVector2 p)
{
	// Scale buffer to hex space
	float hx=((float)p.x_ / scale_.x_);
	float hy=((float)p.y_ / scale_.y_);
	return hexgrid_.CalcPointTile(hx,hy);
}

IntVector2 HexToBuffer::HexCenter(IntVector2 h)
{
	Vector2 c=hexgrid_.CalcTileCenter(h.x_, h.y_);
	c*=scale_;
	return IntVector2((int)c.x_, (int)c.y_);
}

int HexToBuffer::NearestNeighborDirection(IntVector2 p)
{
	IntVector2 h=PointToHex(p);
	IntVector2 c=HexCenter(h);
	
	// Prevent divide-by-zero
	if(p.x_==c.x_) p.x_+=1;
	if(p.y_==c.y_) p.y_+=1;
	
	Vector2 d((float)p.x_ - (float)c.x_, (float)p.y_ - (float)c.y_);
	float am=0.573503f; // sin(30)/cos(30)
	float m=d.y_/d.x_;
	
	if(d.x_<0)
	{
		// Left half of hex
		if(d.y_<0)
		{
			// Top half of hex
			if(m < am) return 1;
			else return 2;
		}
		else
		{
			// Bottom half of hex
			if(std::abs(m) < am) return 1;
			else return 0;
		}
	}
	else
	{
		// Right half of hex
		if(d.y_<0)
		{
			// Top half of hex
			if(std::abs(m) < am) return 4;
			else return 3;
		}
		else
		{
			// Bottom half of hex
			if(m < am) return 4;
			else return 5;
		}
	}
}

float HexToBuffer::BlendMaskValue(IntVector2 p, const NeighborValuesStruct &nv)
{
	float hx=((float)p.x_ / scale_.x_);
	float hy=((float)p.y_ / scale_.y_);
	IntVector2 h=PointToHex(p);
	Vector2 c=hexgrid_.CalcTileCenter(h.x_, h.y_);
	Vector2 v=Vector2(hx,hy)-c;
	
	float val1,val2; // Interpolated values representing the 2 corners of the hex nearest to the point
	
	int nn=NearestNeighborDirection(p);
	Vector2 e1,e2;
	
	if(nn==0)
	{
		val1=(float)std::max(nv.nv_[0], nv.nv_[5]);
		val2=(float)std::max(nv.nv_[0], nv.nv_[1]);
		e1=Vector2(0,1);
		e2=Vector2(-0.8660254f,0.5f);
	}
	else if(nn==1)
	{
		val1=(float)std::max(nv.nv_[1], nv.nv_[0]);
		val2=(float)std::max(nv.nv_[1], nv.nv_[2]);
		e1=Vector2(-0.8660254f,0.5f);
		e2=Vector2(-0.8660254f,-0.5f);
	}
	else if(nn==2)
	{
		val1=(float)std::max(nv.nv_[2], nv.nv_[1]);
		val2=(float)std::max(nv.nv_[2], nv.nv_[3]);
		e1=Vector2(-0.8660254f,-0.5f);
		e2=Vector2(0,-1);
	}
	else if(nn==3)
	{
		val1=(float)std::max(nv.nv_[3], nv.nv_[2]);
		val2=(float)std::max(nv.nv_[3], nv.nv_[4]);
		e1=Vector2(0,-1);
		e2=Vector2(0.8660254f,-0.5f);
	}
	else if(nn==4)
	{
		val1=(float)std::max(nv.nv_[4], nv.nv_[3]);
		val2=(float)std::max(nv.nv_[4], nv.nv_[5]);
		e1=Vector2(0.8660254f,-0.5f);
		e2=Vector2(0.8660254,0.5f);
	}
	else if(nn==5)
	{
		val1=(float)std::max(nv.nv_[5], nv.nv_[4]);
		val2=(float)std::max(nv.nv_[5], nv.nv_[0]);
		e1=Vector2(0.8660254f,0.5f);
		e2=Vector2(0,1);
	}
	
	Vector3 bary=BarycentricCoords(Vector2(0,0), e1, e2, v);
	
	float val=bary.x_ + bary.y_*val1 + bary.z_*val2;
	return val;
	
	/*IntVector2 nb=hexgrid_.CalcNeighbor(h.x_, h.y_, nn);
	Vector2 nc=hexgrid_.CalcTileCenter(nb.x_, nb.y_);
	
	Vector2 axis=nc-c;
	axis.Normalize();
	
	Vector2 v=Vector2(hx,hy)-c;
	Vector2 proj=NearestPoint(axis,v);
	
	float dist=proj.Length();
	return std::min(1.0f, std::max(0.0f, (0.8660254f - dist) / 0.8660254f));*/
}
