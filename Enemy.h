#pragma once
#include "Engine/GameObject.h"

class Enemy : public GameObject
{
public:
	Enemy(GameObject* _pParent, GameObject* _pPlayer, const XMFLOAT3& _position);
	~Enemy();

	void Initialize() override;
	void Update() override;
	void Release() override;
	void Draw() override;

	void OnCollision() override;

	/// <summary>
	/// ダメージを与える
	/// </summary>
	void Damage();

	/// <summary>
	/// 撃っている間か
	/// </summary>
	/// <returns>撃っている間 true / false</returns>
	bool IsFiring() const { return fireTimeLeft_ > 0.0f; }

private:

	int hNormalModel_;  // モデルのハンドル
	int hFireModel_;  // モデルのハンドル

	float fireTimeLeft_;
	int hp_;  // 体力
};
