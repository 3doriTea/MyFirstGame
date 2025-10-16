#pragma once
#include "Engine/GameObject.h"

class Fbx;

class ChildOden : public GameObject
{
public:
	ChildOden(GameObject* _pParent, const DirectX::XMFLOAT3& _offset);
	~ChildOden();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

private:
	Fbx* pFbx_;
};
