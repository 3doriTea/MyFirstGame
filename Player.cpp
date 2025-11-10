#include "Player.h"
#include "Engine/Model.h"
#include "Engine/SphereCollider.h"
#include "Engine/Input.h"

#include "Enemy.h"

#include <format>
#include <cmath>

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
	OutputDebugString(std::format(L"HIT PLAYER \n", _pTarget->name_));
}
