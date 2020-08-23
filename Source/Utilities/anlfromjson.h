#pragma once
#include <Urho3D/Core/Context.h>
#include <Urho3D/Resource/JSONValue.h>
#include "../ThirdParty/accidental-noise-library/anl.h"
using namespace Urho3D;

// Build an ANL Kernel from a JSON descriptor

void BuildANLFromJSON(anl::CKernel &kernel, const JSONValue &json);