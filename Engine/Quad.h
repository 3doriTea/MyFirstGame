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
		DirectX::XMMATRIX matWVP;  // スクリーン空間のやつ
		DirectX::XMMATRIX textureMatrix;  // テクスチャのマトリクス
		DirectX::XMMATRIX matW;  // ワールド行列
		DirectX::XMMATRIX matRotateW;  // 法線用の行列
		DirectX::XMVECTOR lightDir;  // 平行光線の向きベクトル
		DirectX::XMVECTOR lightColor;  // 平行光線の色
		float ambientValue;  // 環境光の明るさ
	};
	struct Vertex
	{
		DirectX::XMVECTOR pos;  // 位置
		DirectX::XMVECTOR uv;  // UV座標
		DirectX::XMVECTOR normal;  // 法線
	};

public:
	Quad();
	virtual ~Quad();
	virtual HRESULT Initialize();
	virtual HRESULT Initialize(const char* _fileName);
	virtual void Draw(const DirectX::XMMATRIX& _worldMatrix);
	virtual void Draw(const DirectX::XMMATRIX& _worldMatrix, const DirectX::XMMATRIX& _uvMatrix);
	void Release();

private:
	Texture* pTexture_;

	ID3D11Buffer* pVertexBuffer_;    // 頂点バッファ
	ID3D11Buffer* pIndexBuffer_;     // インデックスバッファ
	ID3D11Buffer* pConstantBuffer_;  // コンスタントバッファ
};
