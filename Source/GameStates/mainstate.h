#pragma once

#include "../gamestatebase.h"

class MainState : public GameStateBase
{
	URHO3D_OBJECT(MainState, GameStateBase);
	public:
	MainState(Context *context);

	virtual void Start() override;
	virtual void Update(float dt) override;
};
