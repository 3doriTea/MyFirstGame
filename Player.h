#pragma once
#include "Engine/GameObject.h"
#include <format>

class Player : public GameObject
{
public:
	Player(GameObject* _pParent);
	~Player();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void OnCollision(GameObject* _pTarget) override;

private:
	int hModel_;
	float shootTimeLeft_;
	bool onGrounded_;  // ínñ Ç…êGÇÍÇƒÇ¢ÇÈÇ©
	float velocityY_;
};
