#include "SphereCollider.h"

SphereCollider::SphereCollider(
	const DirectX::XMVECTOR _center,
	const float _radius) :
	center_{ _center },
	radius_{ _radius }
{
}

SphereCollider::~SphereCollider()
{
}
