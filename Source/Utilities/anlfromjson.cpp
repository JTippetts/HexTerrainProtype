#include "anlfromjson.h"
#include <vector>

#include <Urho3D/IO/Log.h>

void BuildANLFromJSON(anl::CKernel &kernel, const JSONValue &json)
{
	std::vector<anl::CInstructionIndex> handles;
	
	if(json.IsArray())
	{
		const JSONArray &arr=json.GetArray();
		for(int c=0; c<arr.Size(); ++c)
		{
			const JSONValue &entry=arr[c];
			if(entry.IsArray())
			{
				const JSONArray &entryarr=entry.GetArray();
				int sz=entryarr.Size();
				if(sz>0)
				{
					String n=entryarr[0].GetString();
					if(n=="pi")
					{
						handles.push_back(kernel.pi());
					}
					else if(n=="e")
					{
						handles.push_back(kernel.e());
					}
					else if(n=="one")
					{
						handles.push_back(kernel.one());
					}
					else if(n=="zero")
					{
						handles.push_back(kernel.zero());
					}
					else if(n=="point5")
					{
						handles.push_back(kernel.point5());
					}
					else if(n=="sqrt2")
					{
						handles.push_back(kernel.sqrt2());
					}
					else if(n=="x")
					{
						handles.push_back(kernel.x());
					}
					else if(n=="y")
					{
						handles.push_back(kernel.y());
					}
					else if(n=="z")
					{
						handles.push_back(kernel.z());
					}
					else if(n=="radial")
					{
						handles.push_back(kernel.radial());
					}
					else if(n=="constant")
					{
						if(sz>=2)
						{
							handles.push_back(kernel.constant(entryarr[1].GetDouble()));
						}
						else
						{
							Log::Write(LOG_ERROR, String("Incorrect number of parameters to module ") + n);
						}
					}
					else if(n=="seed")
					{
						if(sz>=2)
						{
							handles.push_back(kernel.seed(entryarr[1].GetUInt()));
						}
						else
						{
							Log::Write(LOG_ERROR, String("Incorrect number of parameters to module ") + n);
						}
					}
					else if(n=="abs")
					{
						if(sz>=2)
						{
							handles.push_back(kernel.abs(handles[entryarr[1].GetUInt()]));
						}
						else
						{
							Log::Write(LOG_ERROR, String("Incorrect number of parameters to module ") + n);
						}
					}
					else if(n=="cos")
					{
						if(sz>=2)
						{
							handles.push_back(kernel.cos(handles[entryarr[1].GetUInt()]));
						}
						else
						{
							Log::Write(LOG_ERROR, String("Incorrect number of parameters to module ") + n);
						}
					}
					else if(n=="sin")
					{
						if(sz>=2)
						{
							handles.push_back(kernel.sin(handles[entryarr[1].GetUInt()]));
						}
						else
						{
							Log::Write(LOG_ERROR, String("Incorrect number of parameters to module ") + n);
						}
					}
					else if(n=="tan")
					{
						if(sz>=2)
						{
							handles.push_back(kernel.tan(handles[entryarr[1].GetUInt()]));
						}
						else
						{
							Log::Write(LOG_ERROR, String("Incorrect number of parameters to module ") + n);
						}
					}
					else if(n=="acos")
					{
						if(sz>=2)
						{
							handles.push_back(kernel.acos(handles[entryarr[1].GetUInt()]));
						}
						else
						{
							Log::Write(LOG_ERROR, String("Incorrect number of parameters to module ") + n);
						}
					}
					else if(n=="asin")
					{
						if(sz>=2)
						{
							handles.push_back(kernel.asin(handles[entryarr[1].GetUInt()]));
						}
						else
						{
							Log::Write(LOG_ERROR, String("Incorrect number of parameters to module ") + n);
						}
					}
					else if(n=="atan")
					{
						if(sz>=2)
						{
							handles.push_back(kernel.atan(handles[entryarr[1].GetUInt()]));
						}
						else
						{
							Log::Write(LOG_ERROR, String("Incorrect number of parameters to module ") + n);
						}
					}
					else if(n=="seeder")
					{
						if(sz>=3)
						{
							handles.push_back(kernel.seeder(handles[entryarr[1].GetUInt()], handles[entryarr[2].GetUInt()]));
						}
						else
						{
							Log::Write(LOG_ERROR, String("Incorrect number of parameters to module ") + n);
						}
					}
					else if(n=="valueBasis")
					{
						if(sz>=3)
						{
							handles.push_back(kernel.valueBasis(handles[entryarr[1].GetUInt()], handles[entryarr[2].GetUInt()]));
						}
						else
						{
							Log::Write(LOG_ERROR, String("Incorrect number of parameters to module ") + n);
						}
					}
					else if(n=="gradientBasis")
					{
						if(sz>=3)
						{
							handles.push_back(kernel.gradientBasis(handles[entryarr[1].GetUInt()], handles[entryarr[2].GetUInt()]));
						}
						else
						{
							Log::Write(LOG_ERROR, String("Incorrect number of parameters to module ") + n);
						}
					}
					else if(n=="simplexBasis")
					{
						if(sz>=2)
						{
							handles.push_back(kernel.simplexBasis(handles[entryarr[1].GetUInt()]));
						}
						else
						{
							Log::Write(LOG_ERROR, String("Incorrect number of parameters to module ") + n);
						}
					}
					else if(n=="add")
					{
						if(sz>=3)
						{
							handles.push_back(kernel.add(handles[entryarr[1].GetUInt()], handles[entryarr[2].GetUInt()]));
						}
						else
						{
							Log::Write(LOG_ERROR, String("Incorrect number of parameters to module ") + n);
						}
					}
					else if(n=="subtract")
					{
						if(sz>=3)
						{
							handles.push_back(kernel.subtract(handles[entryarr[1].GetUInt()], handles[entryarr[2].GetUInt()]));
						}
						else
						{
							Log::Write(LOG_ERROR, String("Incorrect number of parameters to module ") + n);
						}
					}
					else if(n=="multiply")
					{
						if(sz>=3)
						{
							handles.push_back(kernel.multiply(handles[entryarr[1].GetUInt()], handles[entryarr[2].GetUInt()]));
						}
						else
						{
							Log::Write(LOG_ERROR, String("Incorrect number of parameters to module ") + n);
						}
					}
					else if(n=="divide")
					{
						if(sz>=3)
						{
							handles.push_back(kernel.divide(handles[entryarr[1].GetUInt()], handles[entryarr[2].GetUInt()]));
						}
						else
						{
							Log::Write(LOG_ERROR, String("Incorrect number of parameters to module ") + n);
						}
					}
					else if(n=="maximum")
					{
						if(sz>=3)
						{
							handles.push_back(kernel.maximum(handles[entryarr[1].GetUInt()], handles[entryarr[2].GetUInt()]));
						}
						else
						{
							Log::Write(LOG_ERROR, String("Incorrect number of parameters to module ") + n);
						}
					}
					else if(n=="minimum")
					{
						if(sz>=3)
						{
							handles.push_back(kernel.minimum(handles[entryarr[1].GetUInt()], handles[entryarr[2].GetUInt()]));
						}
						else
						{
							Log::Write(LOG_ERROR, String("Incorrect number of parameters to module ") + n);
						}
					}
					else if(n=="pow")
					{
						if(sz>=3)
						{
							handles.push_back(kernel.pow(handles[entryarr[1].GetUInt()], handles[entryarr[2].GetUInt()]));
						}
						else
						{
							Log::Write(LOG_ERROR, String("Incorrect number of parameters to module ") + n);
						}
					}
					else if(n=="bias")
					{
						if(sz>=3)
						{
							handles.push_back(kernel.bias(handles[entryarr[1].GetUInt()], handles[entryarr[2].GetUInt()]));
						}
						else
						{
							Log::Write(LOG_ERROR, String("Incorrect number of parameters to module ") + n);
						}
					}
					else if(n=="gain")
					{
						if(sz>=3)
						{
							handles.push_back(kernel.gain(handles[entryarr[1].GetUInt()], handles[entryarr[2].GetUInt()]));
						}
						else
						{
							Log::Write(LOG_ERROR, String("Incorrect number of parameters to module ") + n);
						}
					}
					else if(n=="scaleDomain")
					{
						if(sz>=3)
						{
							handles.push_back(kernel.scaleDomain(handles[entryarr[1].GetUInt()], handles[entryarr[2].GetUInt()]));
						}
						else
						{
							Log::Write(LOG_ERROR, String("Incorrect number of parameters to module ") + n);
						}
					}
					else if(n=="scaleX")
					{
						if(sz>=3)
						{
							handles.push_back(kernel.scaleX(handles[entryarr[1].GetUInt()], handles[entryarr[2].GetUInt()]));
						}
						else
						{
							Log::Write(LOG_ERROR, String("Incorrect number of parameters to module ") + n);
						}
					}
					else if(n=="scaleY")
					{
						if(sz>=3)
						{
							handles.push_back(kernel.scaleY(handles[entryarr[1].GetUInt()], handles[entryarr[2].GetUInt()]));
						}
						else
						{
							Log::Write(LOG_ERROR, String("Incorrect number of parameters to module ") + n);
						}
					}
					else if(n=="scaleZ")
					{
						if(sz>=3)
						{
							handles.push_back(kernel.scaleZ(handles[entryarr[1].GetUInt()], handles[entryarr[2].GetUInt()]));
						}
						else
						{
							Log::Write(LOG_ERROR, String("Incorrect number of parameters to module ") + n);
						}
					}
					else if(n=="translateDomain")
					{
						if(sz>=3)
						{
							handles.push_back(kernel.translateDomain(handles[entryarr[1].GetUInt()], handles[entryarr[2].GetUInt()]));
						}
						else
						{
							Log::Write(LOG_ERROR, String("Incorrect number of parameters to module ") + n);
						}
					}
					else if(n=="translateX")
					{
						if(sz>=3)
						{
							handles.push_back(kernel.translateX(handles[entryarr[1].GetUInt()], handles[entryarr[2].GetUInt()]));
						}
						else
						{
							Log::Write(LOG_ERROR, String("Incorrect number of parameters to module ") + n);
						}
					}
					else if(n=="translateY")
					{
						if(sz>=3)
						{
							handles.push_back(kernel.translateY(handles[entryarr[1].GetUInt()], handles[entryarr[2].GetUInt()]));
						}
						else
						{
							Log::Write(LOG_ERROR, String("Incorrect number of parameters to module ") + n);
						}
					}
					else if(n=="translateZ")
					{
						if(sz>=3)
						{
							handles.push_back(kernel.translateZ(handles[entryarr[1].GetUInt()], handles[entryarr[2].GetUInt()]));
						}
						else
						{
							Log::Write(LOG_ERROR, String("Incorrect number of parameters to module ") + n);
						}
					}
					else if(n=="tiers")
					{
						if(sz>=3)
						{
							handles.push_back(kernel.tiers(handles[entryarr[1].GetUInt()], handles[entryarr[2].GetUInt()]));
						}
						else
						{
							Log::Write(LOG_ERROR, String("Incorrect number of parameters to module ") + n);
						}
					}
					else if(n=="smoothTiers")
					{
						if(sz>=3)
						{
							handles.push_back(kernel.smoothTiers(handles[entryarr[1].GetUInt()], handles[entryarr[2].GetUInt()]));
						}
						else
						{
							Log::Write(LOG_ERROR, String("Incorrect number of parameters to module ") + n);
						}
					}
					else if(n=="dx")
					{
						if(sz>=3)
						{
							handles.push_back(kernel.dx(handles[entryarr[1].GetUInt()], handles[entryarr[2].GetUInt()]));
						}
						else
						{
							Log::Write(LOG_ERROR, String("Incorrect number of parameters to module ") + n);
						}
					}
					else if(n=="dy")
					{
						if(sz>=3)
						{
							handles.push_back(kernel.dy(handles[entryarr[1].GetUInt()], handles[entryarr[2].GetUInt()]));
						}
						else
						{
							Log::Write(LOG_ERROR, String("Incorrect number of parameters to module ") + n);
						}
					}
					else if(n=="dz")
					{
						if(sz>=3)
						{
							handles.push_back(kernel.dz(handles[entryarr[1].GetUInt()], handles[entryarr[2].GetUInt()]));
						}
						else
						{
							Log::Write(LOG_ERROR, String("Incorrect number of parameters to module ") + n);
						}
					}
					else if(n=="step")
					{
						if(sz>=3)
						{
							handles.push_back(kernel.step(handles[entryarr[1].GetUInt()], handles[entryarr[2].GetUInt()]));
						}
						else
						{
							Log::Write(LOG_ERROR, String("Incorrect number of parameters to module ") + n);
						}
					}
					else if(n=="mix")
					{
						if(sz>=4)
						{
							handles.push_back(kernel.mix(handles[entryarr[1].GetUInt()], handles[entryarr[2].GetUInt()], handles[entryarr[3].GetUInt()]));
						}
						else
						{
							Log::Write(LOG_ERROR, String("Incorrect number of parameters to module ") + n);
						}
					}
					else if(n=="clamp")
					{
						if(sz>=4)
						{
							handles.push_back(kernel.clamp(handles[entryarr[1].GetUInt()], handles[entryarr[2].GetUInt()], handles[entryarr[3].GetUInt()]));
						}
						else
						{
							Log::Write(LOG_ERROR, String("Incorrect number of parameters to module ") + n);
						}
					}
					else if(n=="linearStep")
					{
						if(sz>=4)
						{
							handles.push_back(kernel.linearStep(handles[entryarr[1].GetUInt()], handles[entryarr[2].GetUInt()], handles[entryarr[3].GetUInt()]));
						}
						else
						{
							Log::Write(LOG_ERROR, String("Incorrect number of parameters to module ") + n);
						}
					}
					else if(n=="smoothStep")
					{
						if(sz>=4)
						{
							handles.push_back(kernel.smoothStep(handles[entryarr[1].GetUInt()], handles[entryarr[2].GetUInt()], handles[entryarr[3].GetUInt()]));
						}
						else
						{
							Log::Write(LOG_ERROR, String("Incorrect number of parameters to module ") + n);
						}
					}
					else if(n=="smootherStep")
					{
						if(sz>=4)
						{
							handles.push_back(kernel.smootherStep(handles[entryarr[1].GetUInt()], handles[entryarr[2].GetUInt()], handles[entryarr[3].GetUInt()]));
						}
						else
						{
							Log::Write(LOG_ERROR, String("Incorrect number of parameters to module ") + n);
						}
					}
					else if(n=="randomize")
					{
						if(sz>=4)
						{
							handles.push_back(kernel.randomize(handles[entryarr[1].GetUInt()], handles[entryarr[2].GetUInt()], handles[entryarr[3].GetUInt()]));
						}
						else
						{
							Log::Write(LOG_ERROR, String("Incorrect number of parameters to module ") + n);
						}
					}
					else if(n=="sigmoid")
					{
						if(sz==2)
						{
							handles.push_back(kernel.sigmoid(handles[entryarr[1].GetUInt()]));
						}
						else if(sz>=4)
						{
							handles.push_back(kernel.sigmoid(handles[entryarr[1].GetUInt()], handles[entryarr[2].GetUInt()], handles[entryarr[3].GetUInt()]));
						}
						else
						{
							Log::Write(LOG_ERROR, String("Incorrect number of parameters to module ") + n);
						}
					}
					else if(n=="cellularBasis")
					{
						if(sz>=11)
						{
							handles.push_back(kernel.cellularBasis(handles[entryarr[1].GetUInt()], handles[entryarr[2].GetUInt()], handles[entryarr[3].GetUInt()],handles[entryarr[4].GetUInt()], handles[entryarr[5].GetUInt()], handles[entryarr[6].GetUInt()],
							handles[entryarr[7].GetUInt()], handles[entryarr[8].GetUInt()], handles[entryarr[9].GetUInt()], handles[entryarr[10].GetUInt()]));
						}
						else
						{
							Log::Write(LOG_ERROR, String("Incorrect number of parameters to module ") + n);
						}
					}
					else if(n=="fractal")
					{
						if(sz>=7)
						{
							handles.push_back(kernel.fractal(handles[entryarr[1].GetUInt()], handles[entryarr[2].GetUInt()], handles[entryarr[3].GetUInt()],handles[entryarr[4].GetUInt()], handles[entryarr[5].GetUInt()], handles[entryarr[6].GetUInt()]));
						}
						else
						{
							Log::Write(LOG_ERROR, String("Incorrect number of parameters to module ") + n);
						}
					}
					else if(n=="simplefBm")
					{
						if(sz>=7)
						{
							handles.push_back(kernel.simplefBm(entryarr[1].GetUInt(), entryarr[2].GetUInt(), entryarr[3].GetUInt(),entryarr[4].GetDouble(), entryarr[5].GetUInt(), entryarr[6].GetBool()));
						}
						else
						{
							Log::Write(LOG_ERROR, String("Incorrect number of parameters to module ") + n);
						}
					}
					else if(n=="simpleRidgedMultifractal")
					{
						if(sz>=7)
						{
							handles.push_back(kernel.simpleRidgedMultifractal(entryarr[1].GetUInt(), entryarr[2].GetUInt(), entryarr[3].GetUInt(),entryarr[4].GetDouble(), entryarr[5].GetUInt(), entryarr[6].GetBool()));
						}
						else
						{
							Log::Write(LOG_ERROR, String("Incorrect number of parameters to module ") + n);
						}
					}
					else if(n=="simpleBillow")
					{
						if(sz>=7)
						{
							handles.push_back(kernel.simpleBillow(entryarr[1].GetUInt(), entryarr[2].GetUInt(), entryarr[3].GetUInt(),entryarr[4].GetDouble(), entryarr[5].GetUInt(), entryarr[6].GetBool()));
						}
						else
						{
							Log::Write(LOG_ERROR, String("Incorrect number of parameters to module ") + n);
						}
					}
					else if(n=="color")
					{
						if(sz>=5)
						{
							handles.push_back(kernel.color(entryarr[1].GetFloat(), entryarr[2].GetFloat(), entryarr[3].GetFloat(), entryarr[4].GetFloat()));
						}
						else
						{
							Log::Write(LOG_ERROR, String("Incorrect number of parameters to module ") + n);
						}
					}
					else if(n=="combineRGBA")
					{
						if(sz>=5)
						{
							handles.push_back(kernel.combineRGBA(handles[entryarr[1].GetUInt()], handles[entryarr[2].GetUInt()], handles[entryarr[3].GetUInt()],handles[entryarr[4].GetUInt()]));
						}
						else
						{
							Log::Write(LOG_ERROR, String("Incorrect number of parameters to module ") + n);
						}
					}
				}
				else
				{
					Log::Write(LOG_ERROR, "ANL entry can not be empty.");
				}
			}
			else
			{
				Log::Write(LOG_ERROR, "ANL entry must be an array of format ['module name', index1, index2...]");
			}
		}
	}
	else
	{
		Log::Write(LOG_ERROR, "ANL JSON must be an array.");
	}
}
