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
		//DirectX::XMMATRIX matWVP;  // �X�N���[����Ԃ̂��
		//DirectX::XMMATRIX textureMatrix;  // �e�N�X�`���̃}�g���N�X
		//DirectX::XMMATRIX matW;  // ���[���h�s��
		//DirectX::XMMATRIX matRotateW;  // �@���p�̍s��
		//DirectX::XMVECTOR lightDir;  // ���s�����̌����x�N�g��
		//DirectX::XMVECTOR lightColor;  // ���s�����̐F
		//float ambientValue;  // �����̖��邳
		XMMATRIX matWVP;
		XMMATRIX matNormal;
		XMFLOAT4 diffuse;
		//XMINT4 materialFlag;
		//XMFLOAT4 materialFlag;
		BOOL materialFLag;  // 16byte�P�ʂő����邩��d���Ȃ�

	};

	struct VERTEX
	{
		XMVECTOR position;
		XMVECTOR uv;
		XMVECTOR normal;  // �@��
	};

	int vertexCount_;	// ���_��
	int polygonCount_;	// �|���S����
	int materialCount_;  // �}�e���A���̌�

	ID3D11Buffer* pVertexBuffer_;
	//ID3D11Buffer* pIndexBuffer_;
	// �C���f�b�N�X�o�b�t�@�͊e�}�e���A���������p�ӂ���
	ID3D11Buffer** pIndexBuffer_;
	ID3D11Buffer* pConstantBuffer_;
	std::vector<MATERIAL> materials_;  // �}�e���A����
	std::vector<int> indexCount_;  // �}�e���A�����Ƃ̃C���f�b�N�X��

	VERTEX* pVertexes_;
	int* pIndexes_;
};
