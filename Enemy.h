#pragma once
#include "Engine/GameObject.h"

class Enemy : public GameObject
{
public:
	Enemy(GameObject* _pParent, GameObject* _pPlayer);
	~Enemy();

	void Initialize() override;
	void Update() override;
	void Release() override;
	void Draw() override;

	void OnCollision() override;

	/// <summary>
	/// 撃っている間か
	/// </summary>
	/// <returns>撃っている間 true / false</returns>
	bool IsFiring() const { return fireTimeLeft_ > 0.0f; }

private:
	int hNormalModel_;  // モデルのハンドル
	int hFireModel_;  // モデルのハンドル

	float fireTimeLeft_;
};
