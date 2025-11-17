#include "Enemy.h"
#include "Engine/Model.h"
#include "Engine/SphereCollider.h"
#include "EffectBall.h"

namespace
{
	const int HP{ 5 };
	const int DAMAGE_VALUE{ 1 };
	const float DAMAGE_VALUE_SCALE{ 1.0f };
	const float INIT_SCALE{ 10.0f };
}

Enemy::Enemy(GameObject* _pParent, GameObject* _pPlayer, const XMFLOAT3& _position) :
	GameObject{ _pParent, "Enemy" },
	hNormalModel_{ -1 },
	hFireModel_{ -1 },
	hp_{ HP }
{
	transform_.position_ = _position;
}

Enemy::~Enemy()
{
}

void Enemy::Initialize()
{
	//transform_.position_ = { 0.0f, 0.0f, 50.0f };
	transform_.scale_ = { INIT_SCALE, INIT_SCALE, INIT_SCALE };
	hNormalModel_ = Model::Load("EnemySphere/ESNormal.fbx");
	hFireModel_ = Model::Load("EnemySphere/ESFire.fbx");
	AddCollider(new SphereCollider{ { 0, INIT_SCALE / 2.0f, 0 }, INIT_SCALE });
}

void Enemy::Update()
{
	
}

void Enemy::Release()
{
}

void Enemy::Draw()
{
	transform_.Calculation();
	int hModel{ IsFiring() ? hFireModel_ : hNormalModel_ };
	Model::SetTransform(hModel, transform_);
	Model::Draw(hModel);
}

void Enemy::OnCollision()
{
	OutputDebugString(L"HIT ENEMY \n");
}

void Enemy::Damage()
{
	hp_ -= DAMAGE_VALUE;
	transform_.scale_.x -= DAMAGE_VALUE_SCALE;
	transform_.scale_.y -= DAMAGE_VALUE_SCALE;
	transform_.scale_.z -= DAMAGE_VALUE_SCALE;

	GetCollider()->SetRadius(transform_.scale_.x);

	if (hp_ <= 0)
	{
		DestroyMe();
		EffectBall* pEffectBall{ Instantiate<EffectBall>(GetParent()) };
		pEffectBall->SetPosition(transform_.position_);
	}
}
