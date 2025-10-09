#pragma once
#include "Engine/GameObject.h"

class Fbx;

class Player : public GameObject
{
public:
	Player(GameObject* _pParent);
	~Player();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

private:
	Fbx* pFbx_;
};
