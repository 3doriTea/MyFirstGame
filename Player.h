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

	/// <summary>
	/// “y‘ä‚Ìã‚©”Û‚©
	/// </summary>
	/// <returns>“y‘ä‚Ìã‚Å‚ ‚é true / false</returns>
	bool IsDirectlyGround() const;

	/// <summary>
	/// “y‘ä”ÍˆÍŠO‚Ü‚Å‰Ÿ‚·
	/// </summary>
	void PushToOutsideDirectlyGround();
private:
	int hModel_;
	float shootTimeLeft_;
	bool onGrounded_;  // ’n–Ê‚ÉG‚ê‚Ä‚¢‚é‚©
	float velocityY_;
};
