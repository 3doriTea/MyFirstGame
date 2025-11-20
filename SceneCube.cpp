#include "SceneCube.h"
#include "Engine/Model.h"
#include "Engine/Camera.h"
#include "Engine/Direct3D.h"

namespace
{
	const float FIRE_SPEED{ 1.0f };
	const float DISTANCE{ 2.1f };
	const float DESTROY_DISTANCE{ 100.0f };
}

SceneCube::SceneCube(GameObject* _pParent, const XMFLOAT3& _beginRotation, const bool _reverse) :
	GameObject{ _pParent, "SceneCube" },
	hModel_{ -1 },
	isFired_{ false },
	distance_{ _reverse ? DESTROY_DISTANCE : DISTANCE },
	isReverse_{ _reverse }
{
	transform_.rotate_ = _beginRotation;
	transform_.scale_ = { 1.0f, 1.0f, 1.0f };
}

void SceneCube::Initialize()
{
	hModel_ = Model::Load("SceneCube/SceneCube.fbx");
}

void SceneCube::Update()
{
	//transform_.position_ = Camera::GetPosition();
	XMFLOAT3 targetPos{ Camera::GetTarget() };
	XMFLOAT3 pos{ Camera::GetPosition() };
	XMVECTOR dirV{ XMLoadFloat3(&targetPos) - XMLoadFloat3(&pos) };
	dirV = XMVector3Normalize(dirV);
	dirV *= distance_;
	XMVECTOR posV{ dirV + XMLoadFloat3(&pos) };
	XMStoreFloat3(&transform_.position_, posV);
	//XMVector
	//transform_.rotate_( dirV * 10.0;

	/*static float angle{};
	angle += XM_2PI / 360.0f * 0.1f;
	transform_.rotate_.y = angle;*/
	if (isFired_ == false)
	{
		return;
	}

	if (isReverse_)
	{
		distance_ -= FIRE_SPEED;
	}
	else
	{
		distance_ += FIRE_SPEED;
	}
	if (distance_ > DESTROY_DISTANCE || FIRE_SPEED > distance_)
	{
		DestroyMe();
	}
}

void SceneCube::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Direct3D::SetWriteDepthBuffer(false);
	Direct3D::UseOnceShader(Direct3D::SHADER_BULLET);
	Model::Draw(hModel_);
	Direct3D::SetWriteDepthBuffer(true);
}

void SceneCube::Fire()
{
	isFired_ = true;
}

float SceneCube::GetDestroyTime()
{
	// âΩïbä‘Ç≈è¡Ç≥ÇÍÇÈÇ©
	return DESTROY_DISTANCE / (FIRE_SPEED * 60.0f);
}
