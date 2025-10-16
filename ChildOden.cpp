#include "ChildOden.h"
#include "Engine/Fbx.h"


ChildOden::ChildOden(GameObject* _pParent, const DirectX::XMFLOAT3& _offset) :
	GameObject{ _pParent, "Childen" },
	pFbx_{ nullptr }
{
	transform_.position_ = _offset;
}

ChildOden::~ChildOden()
{
}

void ChildOden::Initialize()
{
	transform_.SetParent(pParent_->GetTransform());

	// TODO: Š‚¤‚È‚ç‚Î new ‚µ‚½‚ ‚ÆŽ¸”s‚·‚é‰Â”\«‚àl—¶‚·‚×‚«
	pFbx_ = new Fbx{};
	pFbx_->Load("Oden.fbx");

	transform_.scale_.x = 0.3f;
	transform_.scale_.y = 0.3f;
	transform_.scale_.z = 0.3f;
}

void ChildOden::Update()
{
	transform_.rotate_.y += 0.1f;
}

void ChildOden::Draw()
{
	if (pFbx_)
	{
		pFbx_->Draw(transform_);
	}
}

void ChildOden::Release()
{
	SAFE_RELEASE(pFbx_);
}
