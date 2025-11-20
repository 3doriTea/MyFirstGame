#pragma once

#include <d3d11.h>
#include <fbxsdk.h>
#include <string>
#include <vector>
#include "Transform.h"
#include "Texture.h"

#pragma comment(lib, "LibFbxSDK-MD.lib")
#pragma comment(lib, "LibXml2-MD.lib")
#pragma comment(lib, "zlib-MD.lib")

class Fbx
{
public:
	Fbx();
	~Fbx();

	HRESULT Load(std::string fileName);
	void    Draw(Transform& transform);
	void    Release();

	void InitVertex(FbxMesh* mesh);
	void InitIndex(FbxMesh* mesh);
	void InitConstant();
	void InitMaterial(FbxNode* _pNode);

private:
	struct MATERIAL
	{
		Texture* pTexture;
		XMFLOAT4 diffuse;
	};

	struct CONSTANT_BUFFER
	{
		//DirectX::XMMATRIX matWVP;  // スクリーン空間のやつ
		//DirectX::XMMATRIX textureMatrix;  // テクスチャのマトリクス
		//DirectX::XMMATRIX matW;  // ワールド行列
		//DirectX::XMMATRIX matRotateW;  // 法線用の行列
		//DirectX::XMVECTOR lightDir;  // 平行光線の向きベクトル
		//DirectX::XMVECTOR lightColor;  // 平行光線の色
		//float ambientValue;  // 環境光の明るさ
		XMMATRIX matWVP;
		XMMATRIX matRotateW;
		XMMATRIX matTexture;
		XMMATRIX matW;
		XMFLOAT4 diffuse;
		XMFLOAT4 lightDir;
		XMFLOAT4 lightColor;
		FLOAT ambientValue;
		BOOL materialFlag;
		//XMFLOAT4 materialFlag;
		//BOOL materialFLag;  // 16byte単位で送られるから仕方ない

	};

	struct VERTEX
	{
		XMVECTOR position;
		XMVECTOR uv;
		XMVECTOR normal;  // 法線
	};

	int vertexCount_;	// 頂点数
	int polygonCount_;	// ポリゴン数
	int materialCount_;  // マテリアルの個数

	ID3D11Buffer* pVertexBuffer_;
	//ID3D11Buffer* pIndexBuffer_;
	// インデックスバッファは各マテリアル分だけ用意する
	ID3D11Buffer** pIndexBuffer_;
	ID3D11Buffer* pConstantBuffer_;
	std::vector<MATERIAL> materials_;  // マテリアルず
	std::vector<int> indexCount_;  // マテリアルごとのインデックス数

	VERTEX* pVertexes_;
	int* pIndexes_;
};
