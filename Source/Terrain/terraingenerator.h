#pragma once

// Terrain Generator Descriptor
// Encapsulate a Terrain Generator, which generates a specific type of terrain.
// Includes an ANL descriptor for the terrain shape, and a list of value->terrain layer mappings to map tiles to terrain types.

#include <vector>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Resource/JSONValue.h>
#include <Urho3D/Resource/Image.h>
#include "terrainlayer.h"

#include <vector>

using namespace Urho3D;

void BuildMask(std::vector<int> &tilemap, int mapwidth, int mapheight, anl::CArray2Dd &mask, int which, float blurfactor);