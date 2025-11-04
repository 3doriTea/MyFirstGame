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
	hModel_ = Model::Load("Snake.fbx");
	AddCollider(new SphereCollider{ {}, 3.0f });
}

void Enemy::Update()
{
	transform_.position_ = {};
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
