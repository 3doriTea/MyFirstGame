#pragma once
#include "Engine/GameObject.h"

class Ground : public GameObject
{
public:
	Ground(GameObject* _pParent);
	~Ground();

	void Initialize() override;
	void Update() override;
	void Draw() override;

private:
	int hModel_;
};
