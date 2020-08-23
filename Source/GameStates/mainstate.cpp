#include "mainstate.h"

#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Audio/SoundSource.h>
#include <Urho3D/Audio/Sound.h>
#include <Urho3D/Audio/Audio.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Graphics/Terrain.h>
#include <Urho3D/Graphics/Light.h>
#include <Urho3D/Graphics/Zone.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Texture2DArray.h>
#include <Urho3D/Graphics/Texture2D.h>
#include <Urho3D/Resource/Image.h>
#include <Urho3D/IO/Filesystem.h>

#include <Urho3D/Resource/JSONFile.h>
#include <Urho3D/Resource/JSONValue.h>


#include "../Components/thirdpersoncamera.h"

#include "../hextobuffer.h"
#include "../Utilities/anlfromjson.h"
#include "../ThirdParty/accidental-noise-library/anl.h"

using namespace Urho3D;


#include <vector>


float rollf(float low, float high);

MainState::MainState(Context *context) : GameStateBase(context)
{
}

void MainState::BuildWaterDepthTexture(TerrainComponents &tcomps, Texture2D *tex)
{
	Image waterdepth(context_);
	waterdepth.SetSize(tcomps.watermap_.GetWidth(), tcomps.watermap_.GetHeight(), 3);
	for(unsigned int x=0; x<waterdepth.GetWidth(); ++x)
	{
		for(unsigned int y=0; y<waterdepth.GetHeight(); ++y)
		{
			float nx=(float)x/(float)(waterdepth.GetWidth());
			float ny=(float)y/(float)(waterdepth.GetHeight());
			
			float ht=RGBToHeight(tcomps.hmap_.GetPixelBilinear(nx,ny));
			float wat=RGBToHeight(tcomps.watermap_.GetPixelBilinear(nx,ny));
			
			float v=std::max(0.0f, std::min(1.0f, (wat-ht)*16.0f));
			waterdepth.SetPixel(x,y,Color(v,0,0));
		}
	}
	tex->SetData(&waterdepth,false);
}

void MainState::Start()
{
	auto cache=GetSubsystem<ResourceCache>();
	
	scene_=SharedPtr<Scene>(new Scene(context_));
	scene_->CreateComponent<Octree>();
	
	cameranode_=scene_->CreateChild();
	ThirdPersonCamera *cam=cameranode_->CreateComponent<ThirdPersonCamera>();
	
	cam->SetClipCamera(false);
	cam->SetSpringTrack(false);
	//cam->SetClipDist(lightingObject["far"]->GetFloat());
	cam->SetClipDist(10000.0f);
	cam->SetCellSize(128);
	cam->SetOrthographic(false);
	cam->SetAllowSpin(true);
	cam->SetAllowPitch(true);
	cam->SetRotAngle(0);
	cam->SetCamAngle(40);
	cam->SetMinFollow(10);
	cam->SetMaxFollow(500);
	cam->SetMinFollowAngle(0);
	cam->SetMaxFollowAngle(89);
	cam->SetPosition(Vector3(0,0,0));
	
	TerrainComponents tcomps(context_, cache);
	tcomps.hmap_.SetSize(4097,4097,3);
	tcomps.watermap_.SetSize(1025,1025,3);
	tcomps.blend0_.SetSize(2048,2048,4);
	tcomps.blend1_.SetSize(2048,2048,4);
	tcomps.mask_.resize(2048,2048);
	
	tcomps.blend0_.Clear(Color(0,0,0,0));
	tcomps.blend1_.Clear(Color(0,0,1,0));
	
	tcomps.watermap_.Clear(Color(0.1,0,0));
	
	terrainnode_=scene_->CreateChild();
	waternode_=scene_->CreateChild();
	Terrain *terrain=terrainnode_->CreateComponent<Terrain>();
	Material *mat=cache->GetResource<Material>("Materials/TerrainEdit8TriplanarBumpReduce.xml");
	
	terrain->SetPatchSize(64);
    terrain->SetSpacing(Vector3(0.25,0.5,0.25));
    terrain->SetSmoothing(false);
	//terrain->SetHeightMap(&hmap);
	//terrain->SetHeightMap(&tcomps.hmap_);
	
	Terrain *water=waternode_->CreateComponent<Terrain>();
	water->SetPatchSize(64);
	water->SetSpacing(Vector3(1,0.5,1));
	water->SetSmoothing(false);
	Material *wmat=cache->GetResource<Material>("Materials/FlowWater.xml");
	waterdepthtex_=SharedPtr<Texture2D>(new Texture2D(context_));
	wmat->SetTexture(TU_SPECULAR, waterdepthtex_);
	water->SetMaterial(wmat);
	
	//water->SetHeightMap(&tcomps.watermap_);
	
	blendtex0_=SharedPtr<Texture2D>(new Texture2D(context_));
	blendtex1_=SharedPtr<Texture2D>(new Texture2D(context_));
	
	blendtex0_->SetData(&tcomps.blend0_);
	blendtex1_->SetData(&tcomps.blend1_);
	
	mat->SetTexture(TU_DIFFUSE, blendtex0_);
	mat->SetTexture(TU_NORMAL, blendtex1_);
	mat->SetTexture(TU_SPECULAR, cache->GetResource<Texture2DArray>("Textures/terrain_diff.xml"));
	mat->SetTexture(TU_EMISSIVE, cache->GetResource<Texture2DArray>("Textures/terrain_normal.xml"));
	
	VectorBuffer buf;
	for(int c=0; c<8; ++c)
	{
		buf.WriteFloat(2.0f);
	}
	Variant var(buf);
	mat->SetShaderParameter("LayerScaling", var);
	
	terrain->SetMaterial(mat);
	terrain->SetCastShadows(true);
	
	
	// Setup zone and lighting
	Node *ln=scene_->CreateChild();
	
	Zone *zone=ln->CreateComponent<Zone>();
	zone->SetAmbientColor(Color(0.6,0.6,0.8));

	zone->SetFogStart(1000);
	zone->SetFogEnd(5000);
	zone->SetFogColor(Color(0.6,0.6,0.8));
	zone->SetBoundingBox(BoundingBox(Vector3(-10000.0f, -600.0f, -10000.0f), Vector3(10000.0f, 600.0f, 10000.0f)));
	
	Light *dl=ln->CreateComponent<Light>();
	dl->SetLightType(LIGHT_DIRECTIONAL);
	ln->SetDirection(Vector3(0.8,-1.0,1.0));
	dl->SetColor(Color(1,1,1));
	dl->SetSpecularIntensity(0.01f);
	dl->SetCastShadows(true);
	
	
	// Test HexToBuffer
	HexToBuffer htb(IntVector2(4097,4097), IntVector2(20,20));
	
	std::vector<int> themap;
	for(int y=0; y<20; ++y)
	{
		for(int x=0; x<20; ++x)
		{
			if(rollf(0,10)<3) themap.push_back(1);
			else themap.push_back(0);
		}
	}
	
	HexGrid hg(1);
	
	for(int x=0; x<4097; ++x)
	{
		for(int y=0; y<4097; ++y)
		{
			IntVector2 h=htb.PointToHex(IntVector2(x,y));
			int mv=themap[h.y_*20+h.x_];
			if(mv==1) tcomps.mask_.set(x,y, 0.0);
			else tcomps.mask_.set(x,y, 1.0);
		}
	}
	
	tcomps.mask_.blur(0.03, false);
	tcomps.mask_.scaleToRange(1.0,0.0);
	
	unsigned int seed=12345;
	ApplyTerrainLayer("testlayer", tcomps, seed);
	terrain->SetHeightMap(&tcomps.hmap_);
	water->SetHeightMap(&tcomps.watermap_);
	BuildWaterDepthTexture(tcomps, waterdepthtex_);
	/*
	
	JSONFile *file=cache->GetResource<JSONFile>("Terrain/testanl.json");
	if(file) BuildANLFromJSON(k, file->GetRoot());
	
	for(int x=0; x<4097; ++x)
	{
		for(int y=0; y<4097; ++y)
		{
			double nx=(double)x/4097.0;
			double ny=(double)y/4097.0;
			double mv=vm.evaluateScalar(nx,ny,k.lastIndex());//thtb.get(x,y);
			tcomps.hmap_.SetPixel(x,y,Color(mv*3,0,0));
		}
	}
	terrain->SetHeightMap(&tcomps.hmap_);
	//thtb.SavePNG(GetSubsystem<FileSystem>()->GetProgramDir() + "thtb.png");
	String fn=GetSubsystem<FileSystem>()->GetProgramDir() + "thtb.png";
	anl::saveDoubleArray(std::string(fn.CString()), &tcomps.mask_);*/
	
}

void MainState::Update(float dt)
{
}
