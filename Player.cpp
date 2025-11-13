#include "Player.h"
#include "Engine/Model.h"
#include "Engine/SphereCollider.h"
#include "Engine/Input.h"

#include "Enemy.h"
#include "SnowBall.h"

#include <format>
#include <cmath>

namespace
{
	const float MOVE_SPEED_PER_FRAME{ 1.0f };
	const float SHOOT_COOL_TIME{ 0.5f };
	const float SHOOT_SPEED{ 3.0f };
	const XMFLOAT3 SHOOT_DIRECTION{ 0.0f, 0.5f, 1.0f };
	const XMVECTOR SHOOT_VELOCITY_V{ XMVector3Normalize(XMLoadFloat3(&SHOOT_DIRECTION)) * SHOOT_SPEED };
	const float FIXED_DELTA_TIME{ 1.0f / 60.0f };
	const float JUMP_POWER{ 10.0f };
	const float FALL_SPEED{ 0.01f };
}

Player::Player(GameObject* _pParent) :
	GameObject{ _pParent, "Player" },
	hModel_{ -1 },
	shootTimeLeft_{ 0.0f }
{
}

Player::~Player()
{
}

void Player::Initialize()
{
	transform_.position_ = { 0.0f, 0.0f, 0.0f };
	hModel_ = Model::Load("Oden.fbx");
	AddCollider(new SphereCollider{ {}, 3.0f });

	transform_.scale_.x = 0.7f;
	transform_.scale_.y = 0.7f;
	transform_.scale_.z = 0.7f;
}

void Player::Update()
{
	transform_.Calculation();

	XMFLOAT3 moveDirection{};

	if (Input::IsKey(DIK_W))
	{
		moveDirection.z += 1.0f;
	}
	if (Input::IsKey(DIK_A))
	{
		moveDirection.x -= 1.0f;
	}
	if (Input::IsKey(DIK_S))
	{
		moveDirection.z -= 1.0f;
	}
	if (Input::IsKey(DIK_D))
	{
		moveDirection.x += 1.0f;
	}

	if (shootTimeLeft_ > 0.0f)
	{
		shootTimeLeft_ -= FIXED_DELTA_TIME;
	}

	if (Input::IsMouseButton(0) && shootTimeLeft_ <= 0.0f)
	{
		XMVECTOR shootVelocity{ SHOOT_VELOCITY_V };

		// ƒvƒŒƒCƒ„[Šî€‚Ì•ûŒü‚É•ÏŠ·
		shootVelocity = XMVector3TransformCoord(shootVelocity, transform_.GetNormalMatrix());

		shootTimeLeft_ += SHOOT_COOL_TIME;
		SnowBall* pSnowBall{ Instantiate<SnowBall>(GetParent()) };
		pSnowBall->SetVelocity(shootVelocity);
		pSnowBall->SetPosition(transform_.position_);
	}

	XMVECTOR mv{ XMLoadFloat3(&moveDirection) * MOVE_SPEED_PER_FRAME };
	mv = XMVector3TransformCoord(mv, transform_.GetNormalMatrix());

	XMVECTOR toPosition{ XMLoadFloat3(&transform_.position_) };

	toPosition += mv;

	XMStoreFloat3(&transform_.position_, toPosition);

	if (transform_.position_.y <= 0.0f)
	{
		onGrounded_ = true;
		velocityY_ = 0.0f;
		transform_.position_.y = 0.0f;
		if (Input::IsKey(DIK_SPACE))
		{
			velocityY_ += JUMP_POWER;
		}
	}
	else
	{
		onGrounded_ = false;
		velocityY_ -= FALL_SPEED;
	}
}

void Player::Draw()
{
	transform_.Calculation();
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Player::Release()
{
}

void Player::OnCollision(GameObject* _pTarget)
{
	//OutputDebugString(std::format(L"HIT PLAYER \n", std::wstring{ _pTarget->name_.begin(), _pTarget->name_.end() }));
}
