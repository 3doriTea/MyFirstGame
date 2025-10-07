#include "Transform.h"

Transform::Transform() :
	position_{ 0, 0, 0 },
	rotate_{ 0, 0, 0 },
	scale_{ 1, 1, 1 },
	matTranslate_{ XMMatrixIdentity() },
	matRotate_{ XMMatrixIdentity() },
	matScale_{ XMMatrixIdentity() }
{
}

Transform::~Transform()
{
}

void Transform::Calculation()
{
	matTranslate_ = DirectX::XMMatrixTranslation(position_.x, position_.y, position_.z);

	matRotate_ = XMMatrixRotationZ(rotate_.z) *
		XMMatrixRotationX(rotate_.x) *
		XMMatrixRotationY(rotate_.y);

	matScale_ = XMMatrixScaling(scale_.x, scale_.y, scale_.z);
}

XMMATRIX Transform::GetWorldMatrix() const
{
	return matScale_ * matRotate_ * matTranslate_;
}

XMMATRIX Transform::GetNormalMatrix() const
{
	return matRotate_ * XMMatrixInverse(nullptr, matScale_);
}
