#include "Ground.h"
#include "Engine/Model.h"

namespace
{
	const float GROUND_SCALE{ 30.0f };
}

Ground::Ground(GameObject* _pParent) :
	GameObject{ _pParent, "Ground" },
	hModel_{ -1 }
{
}

Ground::~Ground()
{
}

void Ground::Initialize()
{
	transform_.scale_ = { GROUND_SCALE, GROUND_SCALE, GROUND_SCALE };
	hModel_ = Model::Load("Ground/Ground2.fbx");
	assert(hModel_ != -1 && "グラウンドモデルの読み込みに失敗");
}

void Ground::Update()
{
}

void Ground::Draw()
{
	transform_.Calculation();
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}
