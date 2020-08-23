// Third person camera
#pragma once

#include <Urho3D/Urho3D.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Scene/LogicComponent.h>
#include <Urho3D/Scene/Node.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Drawable.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Viewport.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Core/Context.h>

using namespace Urho3D;

// Experimental WatchMe camera
class ThirdPersonCamera : public LogicComponent
{
	URHO3D_OBJECT(ThirdPersonCamera, LogicComponent);

	public:
	static void RegisterObject(Context *context);
	ThirdPersonCamera(Context *context);
	virtual void Update(float dt);

	void SetCellSize(unsigned int s){cellsize_=s;SetOrthographic(true);}
	unsigned int GetCellSize(){return cellsize_;}
	void SetCamAngle(float a){camangle_=a;}
	float GetCamAngle(){return camangle_;}
	void SetRotAngle(float a){rotangle_=a;}
	float GetRotAngle(){return rotangle_;}
	void SetClipDist(float c){clipdist_=c;camera_->SetFarClip(clipdist_);}
	float GetClipDist(){return clipdist_;}
	void SetClipCamera(bool c){clipcamera_=c;}
	bool GetClipCamera(){return clipcamera_;}
	void SetSpringTrack(bool s){springtrack_=s;}
	bool GetSpringTrack(){return springtrack_;}
	void SetAllowPitch(bool a){allowpitch_=a;}
	bool GetAllowPitch(){return allowpitch_;}
	void SetAllowSpin(bool a){allowspin_=a;}
	bool GetAllowSpin(){return allowspin_;}
	void SetAllowZoom(bool a){allowzoom_=a;}
	bool GetAllowZoom(){return allowzoom_;}
	void SetOrthographic(bool o);
	bool GetOrthographic(){return orthographic_;}
	void SetOffset(float o){offset_=o;}
	float GetOffset(){return offset_;}
	void SetMinFollow(float f){minfollow_=f; if(follow_<minfollow_) follow_=minfollow_;}
	float GetMinFollow(){return minfollow_;}
	void SetMaxFollow(float f){maxfollow_=f;follow_=maxfollow_;}
	float GetMaxFollow(){return maxfollow_;}
	Viewport *GetViewport(){return viewport_.Get();}
	void SetMinFollowAngle(float a){minfollowangle_=a;}
	void SetMaxFollowAngle(float a){maxfollowangle_=a;}
	float GetMinFollowAngle(){return minfollowangle_;}
	float GetMaxFollowAngle(){return maxfollowangle_;}
	Vector2 GetMouseGround();
	Vector3 GetEyePosition(){return cameranode_->GetWorldPosition();}

	void SetPosition(Vector3 pos);
	Frustum GetFrustum(){if(camera_) return camera_->GetFrustum(); else return Frustum();}

	protected:
	void HandleSetCameraPosition(StringHash eventType, VariantMap &eventData);
	void HandleResetCameraPosition(StringHash eventType, VariantMap &eventData);
	void HandleRequestCameraRotation(StringHash eventType, VariantMap &eventData);
	void HandleShakeCamera(StringHash eventType, VariantMap &eventData);
	void HandleRequestMouseGround(StringHash eventType, VariantMap &eventData);
	void HandleRequestMouseRay(StringHash eventType, VariantMap &eventData);
	void HandleSetCamera(StringHash eventType, VariantMap &eventData);
	void HandleMain(StringHash eventType, VariantMap &eventData);
	void HandlePush(StringHash eventType, VariantMap &eventData);
	virtual void Start();

	Ray GetMouseRay();
	Ray GetScreenRay(int mx, int my);
	bool PickGround(Vector2 &ground, int mx, int my, float maxdistance=1000.0f);
	Vector2 GetScreenGround(int mx, int my);
	float CameraPick(Ray &ray, float followdist);
	void SpringFollow(float dt);
	void SpringPosition(float dt);

	Node *TopLevelNode(Drawable *d, Scene *s);

	unsigned int cellsize_;
	float camangle_, rotangle_;
	float follow_, minfollow_, maxfollow_;
	float minfollowangle_, maxfollowangle_;
	float clipdist_;
	bool clipcamera_;
	bool springtrack_;
	bool allowspin_, allowpitch_, allowzoom_;
	bool orthographic_;

	float curfollow_;
	float followvel_;
	Vector3 pos_;
	Vector3 newpos_;
	Vector3 posvelocity_;
	float offset_;

	float shakemagnitude_, shakespeed_, shaketime_, shakedamping_;

	SharedPtr<Node> shakenode_, anglenode_, cameranode_;
	SharedPtr<Camera> camera_;
	SharedPtr<Viewport> viewport_;
	Vector<WeakPtr<Node>> targets_;
	WeakPtr<Node> maintarget_;

};
