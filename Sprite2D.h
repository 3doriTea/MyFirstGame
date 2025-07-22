#pragma once
#include "Direct3D.h"
#include <DirectXMath.h>
#include "Utility.h"


class Texture;

class Sprite2D
{
private:
	struct CONSTANT_BUFFER
	{
		DirectX::XMMATRIX matWVP;
		DirectX::XMMATRIX matUV;
	};

	struct Vertex
	{
		DirectX::XMVECTOR pos;  // 位置
		DirectX::XMVECTOR uv;  // UV座標
	};

public:
	Sprite2D();
	virtual ~Sprite2D();

	virtual HRESULT Initialize(const char* _fileName);
	virtual void Draw(const RectanInt& _dest);
	virtual void Draw(const RectanInt& _dest, const float _angle);
	virtual void Draw(const RectanInt& _dest, const float _angle, const RectanInt& _src);
	void Release();
private:
	Texture* pTexture_;

	ID3D11Buffer* pVertexBuffer_;    // 頂点バッファ
	ID3D11Buffer* pIndexBuffer_;     // インデックスバッファ
	ID3D11Buffer* pConstantBuffer_;  // コンスタントバッファ
};
