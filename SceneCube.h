#pragma once
#include "Engine/GameObject.h"

class SceneCube : public GameObject
{
public:
	SceneCube(GameObject* _pParent, const XMFLOAT3& _beginRotation, const bool _reverse = false);
	~SceneCube() {}

	void Initialize() override;
	void Update() override;
	void Draw() override;

	void Fire();

	static float GetDestroyTime();

private:
	int hModel_;
	float distance_;
	bool isFired_;
	bool isReverse_;
};
