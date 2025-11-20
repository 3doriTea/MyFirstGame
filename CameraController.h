#pragma once
#include "Engine/GameObject.h"

class Player;

class CameraController : public GameObject
{
public:
	CameraController(GameObject* _pParent);
	~CameraController();

	void Initialize() override;
	void Update() override;

	bool IsInReadyTime() const;
private:
	Player* pPlayer_;
	XMVECTOR prevMousePositionV_;
	XMFLOAT3 mouseDiff_;
	float timeLeft_;
};

