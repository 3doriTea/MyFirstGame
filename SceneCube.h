#pragma once
#include "Engine/GameObject.h"

class SceneCube : public GameObject
{
public:
	SceneCube(GameObject* _pParent, const XMFLOAT3& _beginRotation);
	~SceneCube() {}

	void Initialize() override;
	void Update() override;
	void Draw() override;

private:
	int hModel_;
};
