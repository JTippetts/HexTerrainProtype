#pragma once

#include "../gamestatebase.h"
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Scene/Node.h>
#include <Urho3D/UI/UIElement.h>
#include <Urho3D/Graphics/Texture2D.h>

#include "../Terrain/terrainlayer.h"

class MainState : public GameStateBase
{
	URHO3D_OBJECT(MainState, GameStateBase);
	public:
	MainState(Context *context);

	virtual void Start() override;
	virtual void Update(float dt) override;
	
	protected:
	SharedPtr<Scene> scene_;
	Node *cameranode_;
	Node *terrainnode_, *waternode_;
	
	SharedPtr<Texture2D> blendtex0_, blendtex1_, waterdepthtex_;
	
	void BuildWaterDepthTexture(TerrainComponents &tcomps, Texture2D *tex);
};
