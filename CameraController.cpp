#include "CameraController.h"
#include "Player.h"
#include "Engine/Camera.h"
#include "Engine/Input.h"
#include "Engine/Cursor.h"

#include "SceneCube.h"

namespace
{
	const float DELTA_TIME{ 1.0f / 60.0f };
	const float READY_TIME{ 3.0f };
}

CameraController::CameraController(GameObject* _pParent) :
	GameObject{ _pParent, "CameraController" },
	timeLeft_{ READY_TIME }
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
	if (IsInReadyTime())
	{
		timeLeft_ -= DELTA_TIME;
	}

	XMVECTOR currMousePositionV{ Input::GetMousePosition() };

	Transform* ppTransform{ pPlayer_->GetTransform() };
	assert(ppTransform != nullptr && "プレイヤーのTransformが見つからなかった");

	if (Cursor::IsLock())
	{
		Cursor::SetPositionCenter();

		if (IsInReadyTime() == false)  // 準備時間ではない
		{
			// マウスによる視点と方向操作
			XMVECTOR mouseDiffV{ currMousePositionV - prevMousePositionV_ };
			//XMStoreFloat3(&mouseDiff_, mouseDiffV);
			POINT mouseDiff{ Cursor::GetMove() };
			//ppTransform->rotate_.x -= mouseDiff_.y / 1000.0f;
			ppTransform->rotate_.y += mouseDiff.x / 1000.0f;
		}
	}

	prevMousePositionV_ = currMousePositionV;

	const XMVECTOR CAMERA_OFFSET{ 0, 10.0f, -30.0f, 0 };
	const XMVECTOR CAMERA_TARGET{ 0, 3.0f, 30.0f, 0 };

	XMVECTOR cameraOffset{ CAMERA_OFFSET };

	XMVECTOR playerPosV{ XMLoadFloat3(&ppTransform->position_) };

	XMVECTOR toMovePosV{ cameraOffset };
	toMovePosV = XMVector3TransformCoord(toMovePosV, ppTransform->GetRotateMatrix());
	toMovePosV += playerPosV;

	//transform_.position_.y = ppTransform->position_.y + CAMERA_OFFSET.m128_f32[1];
	XMVECTOR currentPosV{ XMLoadFloat3(&transform_.position_) };
	
	toMovePosV = (toMovePosV - currentPosV) / 4.0f + currentPosV;
	
	toMovePosV.m128_f32[1] = ppTransform->position_.y + cameraOffset.m128_f32[1];


	XMVECTOR targetPosV{ playerPosV + XMVector3TransformCoord(CAMERA_TARGET, ppTransform->GetRotateMatrix()) };
	
	if (IsInReadyTime())
	{
		toMovePosV = targetPosV + XMVECTOR{ 10, 0, 0, 0 };
	}
	else
	{
		SceneCube* pSceneCube{ FindGameObject<SceneCube>() };
		if (pSceneCube)
		{
			pSceneCube->Fire();
		}
	}

	XMStoreFloat3(&transform_.position_, toMovePosV);


	Camera::SetPosition(transform_.position_);

	XMFLOAT3 targetPos{};
	XMStoreFloat3(&targetPos, targetPosV);
	Camera::SetTarget(targetPos);
}

bool CameraController::IsInReadyTime() const
{
	return timeLeft_ > 0.0f;
}
