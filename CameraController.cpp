#include "CameraController.h"
#include "Player.h"
#include "Engine/Camera.h"
#include "Engine/Input.h"

CameraController::CameraController(GameObject* _pParent) :
	GameObject{ _pParent, "CameraController" }
{
}

CameraController::~CameraController()
{
}

void CameraController::Initialize()
{
	pPlayer_ = FindGameObject<Player>();
	assert(pPlayer_ != nullptr && "プレイヤーが見つからなかった");
	prevMousePositionV_ = Input::GetMousePosition();
}

void CameraController::Update()
{
	Transform* ppTransform{ pPlayer_->GetTransform() };
	assert(ppTransform != nullptr && "プレイヤーのTransformが見つからなかった");

	Camera::SetTarget(ppTransform->position_);


	// マウスによる視点と方向操作
	XMVECTOR currMousePositionV{ Input::GetMousePosition() };
	XMVECTOR mouseDiffV{ currMousePositionV - prevMousePositionV_ };
	XMStoreFloat3(&mouseDiff_, mouseDiffV);

	//ppTransform->rotate_.x -= mouseDiff_.y / 1000.0f;
	ppTransform->rotate_.y += mouseDiff_.x / 1000.0f;
	
	prevMousePositionV_ = currMousePositionV;


	const XMVECTOR CAMERA_OFFSET{ 0, 10, -30, 0 };

	XMVECTOR targetPosV{ CAMERA_OFFSET };
	targetPosV = XMVector3TransformCoord(targetPosV, ppTransform->GetRotateMatrix());
	targetPosV += XMLoadFloat3(&ppTransform->position_);

	XMVECTOR currentPosV{ XMLoadFloat3(&transform_.position_) };
	
	XMVECTOR toMovePosV{ (targetPosV - currentPosV) / 4.0f + currentPosV };

	XMStoreFloat3(&transform_.position_, toMovePosV);

	Camera::SetPosition(transform_.position_);
}
