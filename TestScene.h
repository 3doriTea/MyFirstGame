#pragma once
#include "Engine/GameObject.h"

class TestScene : public GameObject
{
public:
	TestScene(GameObject* _pParent);
	~TestScene();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};
