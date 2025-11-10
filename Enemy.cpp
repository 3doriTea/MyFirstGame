#include "Enemy.h"
#include "Engine/Model.h"
#include "Engine/SphereCollider.h"

Enemy::Enemy(GameObject* _pParent) :
	GameObject{ _pParent, "Enemy" },
	hModel_{ -1 }
{
}

Enemy::~Enemy()
{
}

void Enemy::Initialize()
{
	transform_.position_ = { 0.0f, 0.0f, 50.0f };
	hModel_ = Model::Load("Snake.fbx");
	AddCollider(new SphereCollider{ {}, 0.5f });
}

void Enemy::Update()
{
	//transform_.position_ = {};
}

void Enemy::Release()
{
}

void Enemy::Draw()
{
	transform_.Calculation();
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Enemy::OnCollision()
{
	OutputDebugString(L"HIT ENEMY \n");
}
