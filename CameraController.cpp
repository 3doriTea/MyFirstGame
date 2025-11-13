#include "CameraController.h"
#include "Player.h"
#include "Engine/Camera.h"
#include "Engine/Input.h"
#include "Engine/Cursor.h"

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
	if (!Cursor::IsLock())
	{
		return;
	}
	Cursor::SetPositionCenter();

	Transform* ppTransform{ pPlayer_->GetTransform() };
	assert(ppTransform != nullptr && "プレイヤーのTransformが見つからなかった");

	// マウスによる視点と方向操作
	XMVECTOR currMousePositionV{ Input::GetMousePosition() };
	XMVECTOR mouseDiffV{ currMousePositionV - prevMousePositionV_ };
	//XMStoreFloat3(&mouseDiff_, mouseDiffV);
	POINT mouseDiff{ Cursor::GetMove() };
	//ppTransform->rotate_.x -= mouseDiff_.y / 1000.0f;
	ppTransform->rotate_.y += mouseDiff.x / 1000.0f;
	
	prevMousePositionV_ = currMousePositionV;


	const XMVECTOR CAMERA_OFFSET{ 0, 10.0f, -30.0f, 0 };
	const XMVECTOR CAMERA_TARGET{ 0, 3.0f, 30.0f, 0 };

	XMVECTOR playerPosV{ XMLoadFloat3(&ppTransform->position_) };

	XMVECTOR toMovePosV{ CAMERA_OFFSET };
	toMovePosV = XMVector3TransformCoord(toMovePosV, ppTransform->GetRotateMatrix());
	toMovePosV += playerPosV;

	//transform_.position_.y = ppTransform->position_.y + CAMERA_OFFSET.m128_f32[1];
	XMVECTOR currentPosV{ XMLoadFloat3(&transform_.position_) };
	
	toMovePosV = (toMovePosV - currentPosV) / 4.0f + currentPosV;
	
	toMovePosV.m128_f32[1] = ppTransform->position_.y + CAMERA_OFFSET.m128_f32[1];

	XMStoreFloat3(&transform_.position_, toMovePosV);


	XMVECTOR targetPosV{ playerPosV + XMVector3TransformCoord(CAMERA_TARGET, ppTransform->GetRotateMatrix()) };
	
	
	Camera::SetPosition(transform_.position_);

	XMFLOAT3 targetPos{};
	XMStoreFloat3(&targetPos, targetPosV);
	Camera::SetTarget(targetPos);
}
