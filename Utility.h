#pragma once
#include <DirectXMath.h>

struct Vector2Int
{
	int x;
	int y;

	inline DirectX::XMFLOAT2 ToFloat() const
	{
		return { static_cast<float>(x), static_cast<float>(y) };
	}

	inline Vector2Int& operator*=(const int _v)
	{
		x *= _v;
		y *= _v;
		return *this;
	}
	inline Vector2Int& operator/=(const int _v)
	{
		x /= _v;
		y /= _v;
		return *this;
	}
};

inline Vector2Int operator*(const Vector2Int& _vec, const int _v)
{
	return Vector2Int{ _vec } *= _v;
}

inline Vector2Int operator/(const Vector2Int& _vec, const int _v)
{
	return Vector2Int{ _vec } /= _v;
}

struct RectanInt
{
	union
	{
		Vector2Int pivot;
		struct
		{
			int x;
			int y;
		};
	};

	union
	{
		Vector2Int size;
		struct
		{
			int width;
			int height;
		};
	};
};
