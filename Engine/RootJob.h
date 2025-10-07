#pragma once

#include "GameObject.h"

class RootJob final : public GameObject
{
public:
	RootJob(GameObject* _pParent);
	~RootJob();

	void Initialize() override;
	void Update() override;
	void Draw() const override;
	void Release() override;
};
