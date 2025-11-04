#pragma once
#include "Engine/GameObject.h"

class Enemy : public GameObject
{
public:
	Enemy(GameObject* _pParent);
	~Enemy();

	void Initialize() override;
	void Update() override;
	void Release() override;
	void Draw() override;

private:
	int hModel_;  // ƒ‚ƒfƒ‹‚Ìƒnƒ“ƒhƒ‹
};
