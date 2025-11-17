#pragma once
#include "Engine/GameObject.h"

/// <summary>
/// ê·ã 
/// </summary>
class SnowBall : public GameObject
{
public:
	SnowBall(GameObject* _pParent);
	~SnowBall();

	void SetPosition(const XMFLOAT3& _pos);
	void SetVelocity(const XMVECTOR& _v);

	void Initialize() override;
	void Update() override;
	void Draw() override;

	void OnCollision(GameObject* _pGameObject);

private:
	int hModel_;

	XMVECTOR velocityV_;
};
