#include "EffectBall.h"
#include "Engine/Model.h"
#include "Engine/Direct3D.h"

namespace
{
	const float FIXED_DELTA_TIME{ 1.0f / 60.0f };
	const float LIFE_TIME{ 5.0f };
}

EffectBall::EffectBall(GameObject* _pRoot) :
	GameObject{ _pRoot, "EffectBall" },
	hModel_{},
	velocity_{},
	timeLeft_{ LIFE_TIME }
{
}

EffectBall::~EffectBall()
{
}

void EffectBall::SetPosition(const XMFLOAT3& _position)
{
	GetTransform()->position_ = _position;
}

void EffectBall::Initialize()
{
	for (int i = 0; i < EFFECT_COUNT; i++)
	{
		hModel_[i] = Model::Load("SnowBall/SnowBall.fbx");
		Model::TransformAt(hModel_[i]).SetParent(GetTransform());

		float angle{ static_cast<float>(i) / EFFECT_COUNT * XM_2PI };

		velocity_[i] = { std::cosf(angle), 3.0f, std::sinf(angle) };
	}
}

void EffectBall::Update()
{
	enum { X, Y, Z };

	timeLeft_ -= FIXED_DELTA_TIME;
	if (timeLeft_ <= 0.0f)
	{
		DestroyMe();
		return;
	}

	for (int i = 0; i < EFFECT_COUNT; i++)
	{
		XMVECTOR v{ XMLoadFloat3(&Model::TransformAt(hModel_[i]).position_) };
		v += velocity_[i];

		velocity_[i] *= 0.98f;
		if (v.m128_f32[Y] > 0)
		{
			velocity_[i].m128_f32[Y] -= 0.3f;
		}
		else
		{
			velocity_[i].m128_f32[Y] = 0.0f;
			v.m128_f32[Y] = 0.0f;
		}

		XMStoreFloat3(&Model::TransformAt(hModel_[i]).position_, v);
	}
}

void EffectBall::Draw()
{
	for (int i = 0; i < EFFECT_COUNT; i++)
	{
		Direct3D::UseOnceShader(Direct3D::SHADER_BULLET);
		Model::Draw(hModel_[i]);
	}
}
