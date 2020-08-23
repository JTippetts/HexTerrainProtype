#pragma once

#include <Urho3D/Resource/Image.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Resource/ResourceCache.h>

#include "../ThirdParty/accidental-noise-library/anl.h"

using namespace Urho3D;

// Terrain Layer Descriptor
// Encapsulate a terrain layer descriptor
// A Terrain Layer comprises an array of operators that are evaluated in sequence.
// These operators can have different targets depending on the operator.

struct TerrainComponents
{
	Image hmap_, watermap_, blend0_, blend1_;
	anl::CArray2Dd mask_;
	ResourceCache *cache_;
	
	TerrainComponents(Context *context, ResourceCache *cache) : hmap_(context), watermap_(context), blend0_(context), blend1_(context), cache_(cache){}
};

Color HeightToRGB(double ht);
float RGBToHeight(const Color &c);

void ApplyTerrainLayer(const String &name, TerrainComponents &components, unsigned int &seed);
