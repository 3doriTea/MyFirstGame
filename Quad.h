#pragma once
#include "Direct3D.h"
#include <DirectXMath.h>

class Texture;  // 前方宣言

class Quad
{
private:
	// コンスタントバッファー
	struct CONSTANT_BUFFER
	{
		DirectX::XMMATRIX matWVP;
		DirectX::XMMATRIX textureMatrix;
	};
	struct Vertex
	{
		DirectX::XMFLOAT4 pos;  // 位置
		DirectX::XMVECTOR uv;  // UV座標
	};

public:
	Quad();
	virtual ~Quad();
	virtual HRESULT Initialize();
	virtual HRESULT Initialize(const char* _fileName);
	virtual void Draw(DirectX::XMMATRIX& _worldMatrix);
	virtual void Draw(const DirectX::XMMATRIX& _worldMatrix, const DirectX::XMMATRIX& _uvMatrix);
	void Release();

private:
	Texture* pTexture_;

	ID3D11Buffer* pVertexBuffer_;    // 頂点バッファ
	ID3D11Buffer* pIndexBuffer_;     // インデックスバッファ
	ID3D11Buffer* pConstantBuffer_;  // コンスタントバッファ
};
