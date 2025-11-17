#include "SnowBall.h"
#include "Engine/SphereCollider.h"
#include "Engine/Model.h"
#include "Enemy.h"

namespace
{
	const float DEAD_ZONE_Y = -10.0f;
	const char ENEMY_NAME[]{ "Enemy" };
}

SnowBall::SnowBall(GameObject* _pParent) :
	GameObject{ _pParent, "SnowBall" },
	hModel_{ -1 }
{
}

SnowBall::~SnowBall()
{
}

void SnowBall::SetPosition(const XMFLOAT3& _pos)
{
	transform_.position_ = _pos;
}

void SnowBall::SetVelocity(const XMVECTOR& _v)
{
	velocityV_ = _v;
}

void SnowBall::Initialize()
{
	hModel_ = Model::Load("SnowBall/SnowBall.fbx");
	AddCollider(new SphereCollider{ {}, 3.0f });
}

void SnowBall::Update()
{
	if (transform_.position_.y <= DEAD_ZONE_Y)
	{
		DestroyMe();
		return;
	}

	XMVECTOR movedPosition{ XMLoadFloat3(&transform_.position_) + velocityV_ };
	XMStoreFloat3(&transform_.position_, movedPosition);

	velocityV_.m128_f32[1] -= 0.1f;
}

void SnowBall::Draw()
{
	transform_.Calculation();
	Model::SetTransform(hModel_, transform_);
	Direct3D::UseOnceShader(Direct3D::SHADER_BULLET);
	Model::Draw(hModel_);
}

void SnowBall::OnCollision(GameObject* _pGameObject)
{
	if (_pGameObject->GetName() == ENEMY_NAME)
	{
		Enemy* pEnemy{ dynamic_cast<Enemy*>(_pGameObject) };
		pEnemy->Damage();
	}
}
