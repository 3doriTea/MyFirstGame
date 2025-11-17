#pragma once
#include "Engine/GameObject.h"

namespace
{
	const size_t EFFECT_COUNT{ 10 };
}

class EffectBall : public GameObject
{
public:
	EffectBall(GameObject* _pRoot);
	~EffectBall();

	void SetPosition(const XMFLOAT3& _position);

	void Initialize() override;
	void Update() override;
	void Draw() override;

	int hModel_[EFFECT_COUNT];  // モデルの数
	XMVECTOR velocity_[EFFECT_COUNT];  // 速度ベクトル
	float timeLeft_;  // エフェクトを表示させる秒数
};

