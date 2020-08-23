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


#include "../Components/thirdpersoncamera.h"

using namespace Urho3D;

MainState::MainState(Context *context) : GameStateBase(context)
{
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
	cam->SetMaxFollow(100);
	cam->SetMinFollowAngle(0);
	cam->SetMaxFollowAngle(89);
	cam->SetPosition(Vector3(0,0,0));
	
	terrainnode_=scene_->CreateChild();
	Terrain *terrain=terrainnode_->CreateComponent<Terrain>();
	Material *mat=cache->GetResource<Material>("Materials/TerrainEdit8TriplanarBumpReduce.xml");
	
	Image hmap(context_);
	hmap.SetSize(1025,1025,3);
	terrain->SetPatchSize(64);
    terrain->SetSpacing(Vector3(1,0.5,1));
    terrain->SetSmoothing(false);
	terrain->SetHeightMap(&hmap);
	
	blendtex0_=SharedPtr<Texture2D>(new Texture2D(context_));
	blendtex1_=SharedPtr<Texture2D>(new Texture2D(context_));
	
	Image blnd(context_);
	blnd.SetSize(4096,4096,4);
	blnd.Clear(Color(0,0,0,0));
	blendtex0_->SetData(&blnd);
	blnd.Clear(Color(1,0,0,0));
	blendtex1_->SetData(&blnd);
	
	mat->SetTexture(TU_DIFFUSE, blendtex0_);
	mat->SetTexture(TU_NORMAL, blendtex1_);
	mat->SetTexture(TU_SPECULAR, cache->GetResource<Texture2DArray>("Textures/terrain_diff.xml"));
	mat->SetTexture(TU_EMISSIVE, cache->GetResource<Texture2DArray>("Textures/terrain_diff.xml"));
	
	VectorBuffer buf;
	for(int c=0; c<8; ++c)
	{
		buf.WriteFloat(2.0f);
	}
	Variant var(buf);
	mat->SetShaderParameter("LayerScaling", var);
	
	terrain->SetMaterial(mat);
	
	
	// Setup zone and lighting
	Node *ln=scene_->CreateChild();
	
	Zone *zone=ln->CreateComponent<Zone>();
	zone->SetAmbientColor(Color(0.6,0.6,0.8));

	zone->SetFogStart(100);
	zone->SetFogEnd(500);
	zone->SetFogColor(Color(0.6,0.6,0.8));
	zone->SetBoundingBox(BoundingBox(Vector3(-10000.0f, -100.0f, -10000.0f), Vector3(10000.0f, 100.0f, 10000.0f)));
	
	Light *dl=ln->CreateComponent<Light>();
	dl->SetLightType(LIGHT_DIRECTIONAL);
	ln->SetDirection(Vector3(1.5,3.5,-1.5));
	dl->SetColor(Color(2,2,2));
	dl->SetSpecularIntensity(0.01f);
}

void MainState::Update(float dt)
{
}
