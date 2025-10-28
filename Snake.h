#pragma once
#include "Engine/GameObject.h"

class Snake : public GameObject
{
public:
	Snake(GameObject* _pParent);
	~Snake();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

private:
	int hModel_;

	float angle_;
};
