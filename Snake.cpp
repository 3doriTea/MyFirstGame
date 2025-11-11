#include "Snake.h"
#include "Engine/Model.h"

Snake::Snake(GameObject* _pParent) :
	GameObject{ _pParent, "Snake" },
	hModel_{ -1 }
{
}

Snake::~Snake()
{
}

void Snake::Initialize()
{
	hModel_ = Model::Load("Ground/Ground2.fbx");
	//hModel_ = Model::Load("Oden.fbx");
	//hModel_ = Model::Load("Sphere.fbx");
	//hModel_ = Model::Load("Snake.fbx");
}

void Snake::Update()
{
	static const float ROUND_DEGREE{ 360.0f };
	static const float SPEED_DEG_PAR_F{ 0.1f };

	angle_ += SPEED_DEG_PAR_F;
	while (angle_ >= ROUND_DEGREE)
	{
		angle_ -= ROUND_DEGREE;
	}

	transform_.rotate_.y = angle_;
}

void Snake::Draw()
{
	transform_.Calculation();
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Snake::Release()
{
}
