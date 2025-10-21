#include "Player.h"
#include "Engine/Model.h"

Player::Player(GameObject* _pParent) :
	GameObject{ _pParent, "Player" },
	hModel_{ -1 }
{
}

Player::~Player()
{
}

void Player::Initialize()
{
	hModel_ = Model::Load("Oden.fbx");

	transform_.scale_.x = 0.7f;
	transform_.scale_.y = 0.7f;
	transform_.scale_.z = 0.7f;
}

void Player::Update()
{
	transform_.rotate_.y += 0.01f;
	if (transform_.rotate_.y > 60.0f)
	{
		DestroyMe();
	}
}

void Player::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Player::Release()
{
}
