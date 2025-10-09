#include "Player.h"
#include "Engine/Fbx.h"

Player::Player(GameObject* _pParent) :
	GameObject{ _pParent, "Player" },
	pFbx_{ nullptr }
{
}

Player::~Player()
{
}

void Player::Initialize()
{
	// TODO: Š‚¤‚È‚ç‚Î new ‚µ‚½‚ ‚ÆŽ¸”s‚·‚é‰Â”\«‚àl—¶‚·‚×‚«
	pFbx_ = new Fbx{};
	pFbx_->Load("Oden.fbx");

	transform_.scale_.x = 0.7f;
	transform_.scale_.y = 0.7f;
	transform_.scale_.z = 0.7f;
}

void Player::Update()
{
	transform_.rotate_.y += 0.01f;
}

void Player::Draw()
{
	if (pFbx_)
	{
		pFbx_->Draw(transform_);
	}
}

void Player::Release()
{
	SAFE_RELEASE(pFbx_);
}
