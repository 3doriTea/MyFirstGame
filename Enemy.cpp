#include "Enemy.h"
#include "Engine/Model.h"
#include "Engine/SphereCollider.h"

Enemy::Enemy(GameObject* _pParent, GameObject* _pPlayer) :
	GameObject{ _pParent, "Enemy" },
	hNormalModel_{ -1 },
	hFireModel_{ -1 }
{
}

Enemy::~Enemy()
{
}

void Enemy::Initialize()
{
	transform_.position_ = { 0.0f, 0.0f, 50.0f };
	transform_.scale_ = { 10.0f, 10.0f, 10.0f };
	hNormalModel_ = Model::Load("EnemySphere/ESNormal.fbx");
	hFireModel_ = Model::Load("EnemySphere/ESFire.fbx");
	AddCollider(new SphereCollider{ {}, 0.5f });
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
	Direct3D::UseOnceShader(Direct3D::SHADER_BULLET);
	Model::Draw(hModel);
}

void Enemy::OnCollision()
{
	OutputDebugString(L"HIT ENEMY \n");
}
