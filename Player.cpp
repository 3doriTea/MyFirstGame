#include "Player.h"
#include "Engine/Model.h"
#include "Engine/SphereCollider.h"
#include "Engine/Input.h"

#include "Enemy.h"

#include <format>
#include <cmath>

namespace
{
	const float MOVE_SPEED_PER_FRAME{ 1.0f };
}

Player::Player(GameObject* _pParent) :
	GameObject{ _pParent, "Player" },
	hModel_{ -1 }
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

	if (Input::IsKey(DIK_LEFTARROW))
	{
		transform_.rotate_.y += 0.001f * (180.0f / XM_PI);
	}
	if (Input::IsKey(DIK_RIGHTARROW))
	{
		transform_.rotate_.y -= 0.001f * (180.0f / XM_PI);
	}

	XMVECTOR mv{ XMLoadFloat3(&moveDirection) * MOVE_SPEED_PER_FRAME };

	mv = XMVector3TransformCoord(mv, transform_.GetNormalMatrix());

	XMVECTOR toPosition{ XMLoadFloat3(&transform_.position_) };

	toPosition += mv;

	XMStoreFloat3(&transform_.position_, toPosition);

	return;

	Enemy* enemy{ FindGameObject<Enemy>()};

	XMVECTOR ePos{ XMLoadFloat3(&enemy->GetTransform()->position_) };
	XMVECTOR pPos{ XMLoadFloat3(&transform_.position_) };

	float distance{ XMVector3Length(ePos - pPos).m128_f32[0] };

	OutputDebugString(std::format(L"{}m\n", distance).c_str());
	//OutputDebugString(std::format(L"{}m\n", distance).c_str());

	if (Input::IsKey(DIK_X))
	{
		transform_.position_.z += 0.1f;
	}

	return;

	static float angle{};
	angle += XM_2PI / 120;
	while (angle >= XM_2PI)
	{
		angle -= XM_2PI;
	}
	transform_.position_.x = 6.0f * std::sinf(angle) - 3.0f;
	transform_.position_.z = 6.0f * std::cosf(angle) - 3.0f;

	transform_.rotate_.y += 0.1f;
	if (transform_.rotate_.y > 60.0f)
	{
		DestroyMe();
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
