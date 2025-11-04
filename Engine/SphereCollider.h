#pragma once
#include "Direct3D.h"

/// <summary>
/// ‹…‚Ì“–‚½‚è”»’è
/// </summary>
class SphereCollider
{
public:
	SphereCollider(const DirectX::XMVECTOR _center, const float _radius);
	~SphereCollider();

	float GetRadius() const { return radius_; }
	DirectX::XMVECTOR GetCenter() const { return center_; }

private:
	DirectX::XMVECTOR center_;  // ‹…‚Ì’†S
	float radius_;              // ‹…‚Ì”¼Œa
};
