#include "gamestatehandler.h"

#include <Urho3D/IO/Log.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/Graphics/Texture2D.h>

GameStateHandler::GameStateHandler(Context *context) : Object(context), fade_(FadedIn), duration_(0.5f)
{
	SubscribeToEvent(StringHash("Update"), URHO3D_HANDLER(GameStateHandler, HandleUpdate));
	SubscribeToEvent(StringHash("EndFrame"), URHO3D_HANDLER(GameStateHandler, HandleEndFrame));
}

void GameStateHandler::SwitchToState(SharedPtr<GameStateBase> level)
{
	if(fade_==FadingIn || fade_==FadingOut) return;
	if(!level) return;

	auto cache=GetSubsystem<ResourceCache>();
	auto graphics=GetSubsystem<Graphics>();
	auto ui=GetSubsystem<UI>();

	element_=SharedPtr<BorderImage>(new BorderImage(context_));
	element_->SetTexture(cache->GetResource<Texture2D>("Textures/UI.png"));
	element_->SetImageRect(IntRect(86,90,87,91));
	element_->SetSize(ui->GetRoot()->GetSize());
	element_->SetColor(Color(0,0,0));
	ui->GetRoot()->GetChild("FadeLayer", true)->AddChild(element_);

	fade_=FadingOut;
	counter_=0.0f;
	nextstate_=level;
}

void GameStateHandler::HandleUpdate(StringHash eventType, VariantMap &eventData)
{
	static StringHash TimeStep("TimeStep");
	float dt=eventData[TimeStep].GetFloat();
	if(fade_==FadedIn)
	{
		if(currentstate_) currentstate_->Update(dt);
		return; // Do nothing if faded in
	}

	counter_+=dt;
	if(counter_ >= duration_)
	{
		// Done fading
		if(fade_==FadingIn)
		{
			// Fully faded in, remove the element and return
			element_->Remove();
			element_.Reset();
			fade_=FadedIn;
			return;
		}
		else if(fade_==FadingOut)
		{
			fade_=FadedOut;
			return;
		}
	}

	float opacity=counter_ / duration_;
	if(fade_==FadingIn) opacity=1.0f-opacity;

	element_->SetOpacity(opacity);

}

void GameStateHandler::HandleEndFrame(StringHash eventType, VariantMap &eventData)
{
	if(fade_==FadedOut)
	{
		// Okay to switch
		if(currentstate_)
		{	Log::Write(LOG_INFO, "Stopping previous game state");
			currentstate_->Stop();
			currentstate_.Reset();
		}
		currentstate_=nextstate_;
		Log::Write(LOG_INFO, "Starting next game state.");
		currentstate_->Start();

		fade_=FadingIn;
		counter_=0;
	}
}
