#include "terrainlayer.h"

#include <Urho3D/Resource/JSONFile.h>
#include <Urho3D/Resource/JSONValue.h>
#include <Urho3D/IO/Log.h>
#include "../Utilities/anlfromjson.h"
#include <cmath>

Color HeightToRGB(double ht)
{
	double expht=std::floor(ht*255.0);
	double rm=ht*255.0-expht;
	return Color(expht/255.0f, rm, 0);
}

float RGBToHeight(const Color &c)
{
	 return c.r_+c.g_/255.0f;
}

void BalanceColors(anl::SRGBA &col0, anl::SRGBA &col1, int layer)
{
	// Balance colors around the given channel
    if(layer==0)
    {
        col0.r=std::min(1.0f,col0.r);
        float others=col0.g+col0.b+col0.a+col1.r+col1.g+col1.b+col1.a;
        col0.g=(col0.g/others)*(1.0f-col0.r);
        col0.b=(col0.b/others)*(1.0f-col0.r);
        col0.a=(col0.a/others)*(1.0f-col0.r);
        col1.r=(col1.r/others)*(1.0f-col0.r);
        col1.g=(col1.g/others)*(1.0f-col0.r);
        col1.b=(col1.b/others)*(1.0f-col0.r);
        col1.a=(col1.a/others)*(1.0f-col0.r);
    }
    else if(layer==1)
    {
        col0.g=std::min(1.0f,col0.g);
        float others=col0.r+col0.b+col0.a+col1.r+col1.g+col1.b+col1.a;
        col0.r=(col0.r/others)*(1.0f-col0.g);
        col0.b=(col0.b/others)*(1.0f-col0.g);
        col0.a=(col0.a/others)*(1.0f-col0.g);
        col1.r=(col1.r/others)*(1.0f-col0.g);
        col1.g=(col1.g/others)*(1.0f-col0.g);
        col1.b=(col1.b/others)*(1.0f-col0.g);
        col1.a=(col1.a/others)*(1.0f-col0.g);
    }
    else if(layer==2)
    {
        col0.b=std::min(1.0f,col0.b);
        float others=col0.r+col0.g+col0.a+col1.r+col1.g+col1.b+col1.a;
        col0.r=(col0.r/others)*(1.0f-col0.b);
        col0.g=(col0.g/others)*(1.0f-col0.b);
        col0.a=(col0.a/others)*(1.0f-col0.b);
        col1.r=(col1.r/others)*(1.0f-col0.b);
        col1.g=(col1.g/others)*(1.0f-col0.b);
        col1.b=(col1.b/others)*(1.0f-col0.b);
        col1.a=(col1.a/others)*(1.0f-col0.b);
    }
    else if(layer==3)
    {
        col0.a=std::min(1.0f,col0.a);
        float others=col0.r+col0.g+col0.b+col1.r+col1.g+col1.b+col1.a;
        col0.r=(col0.r/others)*(1.0f-col0.a);
        col0.g=(col0.g/others)*(1.0f-col0.a);
        col0.b=(col0.b/others)*(1.0f-col0.a);
        col1.r=(col1.r/others)*(1.0f-col0.a);
        col1.g=(col1.g/others)*(1.0f-col0.a);
        col1.b=(col1.b/others)*(1.0f-col0.a);
        col1.a=(col1.a/others)*(1.0f-col0.a);
    }
    else if(layer==4)
    {
        col1.r=std::min(1.0f,col1.r);
        float others=col1.g+col1.b+col1.a+col0.r+col0.g+col0.b+col0.a;
        col1.g=(col1.g/others)*(1.0f-col1.r);
        col1.b=(col1.b/others)*(1.0f-col1.r);
        col1.a=(col1.a/others)*(1.0f-col1.r);
        col0.r=(col0.r/others)*(1.0f-col1.r);
        col0.g=(col0.g/others)*(1.0f-col1.r);
        col0.b=(col0.b/others)*(1.0f-col1.r);
        col0.a=(col0.a/others)*(1.0f-col1.r);
    }
    else if(layer==5)
    {
        col1.g=std::min(1.0f,col1.g);
        float others=col1.r+col1.b+col1.a+col0.r+col0.g+col0.b+col0.a;
        col1.r=(col1.r/others)*(1.0f-col1.g);
        col1.b=(col1.b/others)*(1.0f-col1.g);
        col1.a=(col1.a/others)*(1.0f-col1.g);
        col0.r=(col0.r/others)*(1.0f-col1.g);
        col0.g=(col0.g/others)*(1.0f-col1.g);
        col0.b=(col0.b/others)*(1.0f-col1.g);
        col0.a=(col0.a/others)*(1.0f-col1.g);
    }
    else if(layer==6)
    {
        col1.b=std::min(1.0f,col1.b);
        float others=col1.r+col1.g+col1.a+col0.r+col0.g+col0.b+col0.a;
        col1.r=(col1.r/others)*(1.0f-col1.b);
        col1.g=(col1.g/others)*(1.0f-col1.b);
        col1.a=(col1.a/others)*(1.0f-col1.b);
        col0.r=(col0.r/others)*(1.0f-col1.b);
        col0.g=(col0.g/others)*(1.0f-col1.b);
        col0.b=(col0.b/others)*(1.0f-col1.b);
        col0.a=(col0.a/others)*(1.0f-col1.b);
    }
    else
    {
        col1.a=std::min(1.0f,col1.a);
        float others=col1.r+col1.g+col1.b+col0.r+col0.g+col0.b+col0.a;
        col1.r=(col1.r/others)*(1.0f-col1.a);
        col1.g=(col1.g/others)*(1.0f-col1.a);
        col1.b=(col1.b/others)*(1.0f-col1.a);
        col0.r=(col0.r/others)*(1.0f-col1.a);
        col0.g=(col0.g/others)*(1.0f-col1.a);
        col0.b=(col0.b/others)*(1.0f-col1.a);
        col0.a=(col0.a/others)*(1.0f-col1.a);
    }
}
	

void CopyBlendBuffer(anl::CArray2Drgba &blend0, anl::CArray2Drgba &blend1, anl::CArray2Dd &arr, int which)
{
	int w=std::min(blend0.width(), arr.width());
	int h=std::min(blend0.height(), arr.height());
	for(int y=0; y<h; ++y)
	{
		for(int x=0; x<w; ++x)
		{
			double v=arr.get(x,y);
			anl::SRGBA rgba0=blend0.get(x,y);
			anl::SRGBA rgba1=blend1.get(x,y);
			switch(which)
			{
				case 0: rgba0.r=v; break;
				case 1: rgba0.g=v; break;
				case 2: rgba0.b=v; break;
				case 3: rgba0.a=v; break;
				case 4: rgba1.r=v; break;
				case 5: rgba1.g=v; break;
				case 6: rgba1.b=v; break;
				case 7: rgba1.a=v; break;
				default: rgba0.r=v; break;
			};
			
			BalanceColors(rgba0, rgba1, which);
			blend0.set(x,y,rgba0);
			blend1.set(x,y,rgba1);
		}
	}
}

void ApplyTerrainLayer(const String &name, TerrainComponents &components, unsigned int &seed)
{
	// set up some scratch buffers
	anl::CArray2Dd hmap(components.hmap_.GetWidth(), components.hmap_.GetHeight());
	anl::CArray2Dd water(components.watermap_.GetWidth(), components.watermap_.GetHeight());
	anl::CArray2Dd mask(components.mask_.width(), components.mask_.height());
	anl::CArray2Drgba blend0(components.blend0_.GetWidth(), components.blend0_.GetHeight()), blend1(components.blend0_.GetWidth(), components.blend0_.GetHeight());
	
	// Open JSON file corresponding to terrain
	JSONFile *file=components.cache_->GetResource<JSONFile>(String("Terrain/") + name + String(".json"));
	if(file)
	{
		const JSONValue &json=file->GetRoot();
		if(json.IsArray())
		{
			const JSONArray &arr=json.GetArray();
			for(int c=0; c<arr.Size(); ++c)
			{
				if(arr[c].IsArray())
				{
					const JSONArray &entry=arr[c].GetArray();
					if(entry.Size()!=0)
					{
						// First element is operation
						String op=entry[0].GetString();
						if(op=="MapNoise")
						{
							if(entry.Size()>=3)
							{
								// Second element is target, third element is ANL descriptor array
								String tar=entry[1].GetString();
								
								if(tar=="Height")
								{
									// Map to height buffer
									anl::CKernel k;
								
									BuildANLFromJSON(k, entry[2]);
									k.seeder(k.seed(seed++), k.lastIndex());  // Attach a seeder to the noise module
									anl::map2DNoZ(anl::SEAMLESS_NONE, hmap, k, anl::SMappingRanges(), k.lastIndex());
									
									// fourth and fifth elements are remap range if present
									if(entry.Size()==5)
									{
										double low=entry[3].GetDouble();
										double high=entry[4].GetDouble();
										hmap.scaleToRange(low,high);
									}
								}
								else if(tar=="Water")
								{
									// Map to height buffer
									anl::CKernel k;
								
									BuildANLFromJSON(k, entry[2]);
									k.seeder(k.seed(seed++), k.lastIndex());  // Attach a seeder to the noise module
									anl::map2DNoZ(anl::SEAMLESS_NONE, water, k, anl::SMappingRanges(), k.lastIndex());
									
									// fourth and fifth elements are remap range if present
									if(entry.Size()==5)
									{
										double low=entry[3].GetDouble();
										double high=entry[4].GetDouble();
										water.scaleToRange(low,high);
									}
								}
								else if(tar=="Layer0")
								{
									// Map to height buffer
									anl::CKernel k;
								
									BuildANLFromJSON(k, entry[2]);
									k.seeder(k.seed(seed++), k.lastIndex());  // Attach a seeder to the noise module
									anl::mapRGBA2DNoZ(anl::SEAMLESS_NONE, blend0, k, anl::SMappingRanges(), k.lastIndex());
								}
								
								else if(tar=="Layer1")
								{
									// Map to height buffer
									anl::CKernel k;
								
									BuildANLFromJSON(k, entry[2]);
									k.seeder(k.seed(seed++), k.lastIndex());  // Attach a seeder to the noise module
									anl::mapRGBA2DNoZ(anl::SEAMLESS_NONE, blend1, k, anl::SMappingRanges(), k.lastIndex());
								}
							}
							else
							{
								Log::Write(LOG_ERROR, "MapNoise op must have 3 elements.");
							}
						}
					}
					else
					{
						Log::Write(LOG_ERROR, "Terrain layer entry is empty.");
					}
				}
				else
				{
					Log::Write(LOG_ERROR, "Terrain layer element must be an array.");
				}
			}
		}
		else
		{
			Log::Write(LOG_ERROR, "Terrain layer JSON must be an array.");
		}
	}
	else
	{
		Log::Write(LOG_ERROR, String("Could not open terrain layer ") + name);
	}
	
	
	// Copy data into components.
	for(int y=0; y<hmap.height(); ++y)
	{
		for(int x=0; x<hmap.width(); ++x)
		{
			float mx=(float)x/(float)hmap.width();
			float my=(float)y/(float)hmap.height();
			float maskval=components.mask_.getBilinear(mx,my);
			
			float oldht=RGBToHeight(components.hmap_.GetPixel(x,y));
			float newht=(float)hmap.get(x,y);
			
			components.hmap_.SetPixel(x,y,HeightToRGB(oldht+maskval*(newht-oldht)));
		}
	}
	for(int y=0; y<water.height(); ++y)
	{
		for(int x=0; x<water.width(); ++x)
		{
			float mx=(float)x/(float)water.width();
			float my=(float)y/(float)water.height();
			float maskval=components.mask_.getBilinear(mx,my);
			
			float oldht=RGBToHeight(components.watermap_.GetPixel(x,y));
			float newht=(float)water.get(x,y);
			
			components.watermap_.SetPixel(x,y,HeightToRGB(oldht+maskval*(newht-oldht)));
		}
	}
	for(int y=0; y<blend0.height(); ++y)
	{
		for(int x=0; x<blend0.width(); ++x)
		{
			float mx=(float)x/(float)blend0.width();
			float my=(float)y/(float)blend0.height();
			float maskval=components.mask_.getBilinear(mx,my);
			
			Color c0=components.blend0_.GetPixel(x,y);
			anl::SRGBA oldc0(c0.r_,c0.g_,c0.b_,c0.a_);
			anl::SRGBA newc0=blend0.get(x,y);
			
			Color c1=components.blend1_.GetPixel(x,y);
			anl::SRGBA oldc1(c1.r_,c1.g_,c1.b_,c1.a_);
			anl::SRGBA newc1=blend1.get(x,y);
			
			anl::SRGBA nc0=oldc0 + (newc0-oldc0)*maskval;
			anl::SRGBA nc1=oldc1 + (newc1-oldc1)*maskval;
			
			components.blend0_.SetPixel(x,y,Color(nc0.r,nc0.g,nc0.b,nc0.a));
			components.blend1_.SetPixel(x,y,Color(nc1.r,nc1.g,nc1.b,nc1.a));
			
			
		}
	}
}
